// FBNetwork - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <WString.h>  // Arduino
#include <iterator>   //
#include <list>

#include <WoodBlock/Macro.h>
#include <WoodBlock/Namespace.hpp>

#include <WoodBlock/Core/FBInstance.h>
#include <WoodBlock/Core/FBType.h>
#include <WoodBlock/Core/ManagedConnection.h>

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

class FBNetwork {
 public:
  FBNetwork() : fbInstances(), eventConnections(), dataConnections() {}
  ~FBNetwork() {
    disconnect();

    for (std::list<DataConnection*>::iterator it = dataConnections.begin();
         it != dataConnections.end(); ++it) {
      delete *it;
    }
    dataConnections.clear();

    for (std::list<EventConnection*>::iterator it = eventConnections.begin();
         it != eventConnections.end(); ++it) {
      delete *it;
    }
    eventConnections.clear();

    for (std::list<FBInstance*>::iterator it = fbInstances.begin();
         it != fbInstances.end(); ++it) {
      delete *it;
    }
    fbInstances.clear();
  }

  FBInstance* findFBInstanceByName(const String& fbInstanceName) {
    for (std::list<FBInstance*>::iterator it = fbInstances.begin();
         it != fbInstances.end(); ++it) {
      if ((*it)->getName().equals(fbInstanceName)) {
        return *it;
      }
    }
    return nullptr;
  }
  // Note: Don't delete a managed FBInstance directly. Please call
  // detachAndDeleteFBInstance()!
  bool attachFBInstance(FBInstance* fbInstance) {
    if (fbInstance) {
      fbInstances.push_back(fbInstance);
      return true;
    } else {
      return false;
    }
  }
  bool detachAndDeleteFBInstance(const String& fbInstanceName) {
    for (std::list<FBInstance*>::iterator it = fbInstances.begin();
         it != fbInstances.end(); ++it) {
      if ((*it)->getName().equals(fbInstanceName)) {
        disconnectFBInstance(**it);
        delete *it;
        fbInstances.erase(it);  // it =
        return true;
      }
    }
    return false;
  }

  // TODO: used initializer_list. reference
  // https://songlee24.github.io/2014/07/22/cpp-changeable-parameter/
  bool connect(const char* srcFBInstanceName, const char* outEventName,
               const char* outVariableNames[], int sizeofOutVariables,
               const char* destFBInstanceName, const char* inEventName,
               const char* inVariableNames[], int sizeofInVariables) {
    WB_CHECK_PTR_RETURN_VALUE(srcFBInstanceName, false);
    WB_CHECK_PTR_RETURN_VALUE(outEventName, false);
    WB_CHECK_PTR_RETURN_VALUE(outVariableNames, false);
    WB_CHECK_EXP_RETURN_VALUE((sizeofOutVariables <= 0), false);
    WB_CHECK_PTR_RETURN_VALUE(destFBInstanceName, false);
    WB_CHECK_PTR_RETURN_VALUE(inEventName, false);
    WB_CHECK_PTR_RETURN_VALUE(inVariableNames, false);
    WB_CHECK_EXP_RETURN_VALUE((sizeofInVariables <= 0), false);
    WB_CHECK_EXP_RETURN_VALUE((sizeofOutVariables != sizeofInVariables), false);

    FBInstance* sourceFBInstance = findFBInstanceByName(srcFBInstanceName);
    FBInstance* destFBInstance = findFBInstanceByName(destFBInstanceName);
    if (sourceFBInstance == nullptr) {
      // TODO: printf(WARNING, "sourceFBInstance is nullptr!");
      return false;
    }
    if (destFBInstance == nullptr) {
      // TODO: printf(WARNING, "destFBInstance is nullptr!");
      return false;
    }
    bool result = connectEvent(*sourceFBInstance, outEventName,
                               outVariableNames, *destFBInstance, inEventName,
                               inVariableNames, sizeofInVariables);
    if (!result) {
      // TODO: printf(HINT, "connectTo() is false!");
      return false;
    }
    return true;
  }

  void disconnect() {
    for (std::list<FBInstance*>::iterator it = fbInstances.begin();
         it != fbInstances.end(); ++it) {
      disconnectFBInstance(**it);
    }
  }

  // ====================== execute =========================
  static void handleEventOutputCallback(FBNetwork& fbNetwork,
                                        FBInstance& fbInstance,
                                        EventOutput& eventOutput) {
    // FBNetwork* _fbNetwork = (FBNetwork*)fbNetwork;
    fbNetwork._handleEventOutputCallback(fbInstance, eventOutput);
  }
  static OutputVariable* searchOutDataCallback(FBNetwork& fbNetwork,
                                               EventConnection& eventConnect,
                                               FBInstance& fbInstance,
                                               EventInput& eventInput,
                                               InputVariable& inputVariable) {
    // FBNetwork* _fbNetwork = (FBNetwork*)fbNetwork;
    //  EventConnection* _eventConnect = (EventConnection*)eventConnect;
    return fbNetwork._searchOutDataCallback(eventConnect, fbInstance,
                                            eventInput, inputVariable);
  }

  bool fetchExternalEvents() {
    bool result = false;
    for (std::list<FBInstance*>::iterator it = fbInstances.begin();
         it != fbInstances.end(); ++it) {
      result |= (*it)->getFBType().fetchExternalEvents(
          *this, **it, FBNetwork::handleEventOutputCallback);
    }
    return result;
  }

  // TODO: move to private
  void _handleEventOutputCallback(FBInstance& fbInstance,
                                  EventOutput& eventOutput) {
    for (std::list<EventConnection*>::iterator it = eventConnections.begin();
         it != eventConnections.end(); ++it) {
      if (((*it)->getSourceFBInstance() == &fbInstance) &&
          ((*it)->getSourceEvent() == &eventOutput)) {
        (*it)->getDestinationFBInstance()->getFBType().processEventInput(
            *this, **it, *((*it)->getDestinationFBInstance()),
            *((*it)->getDestinationEvent()), FBNetwork::searchOutDataCallback,
            FBNetwork::handleEventOutputCallback);
      }
    }
  }
  // TODO: move to private
  OutputVariable* _searchOutDataCallback(EventConnection& eventConnect,
                                         FBInstance& fbInstance,
                                         EventInput& eventInput,
                                         InputVariable& inputVariable) {
    for (std::list<DataConnection*>::iterator it = dataConnections.begin();
         it != dataConnections.end(); ++it) {
      if (((*it)->eventConnection() == &eventConnect) &&
          ((*it)->destination() == &inputVariable)) {
        return (*it)->source();
      }
    }
    return nullptr;
  }

 private:
  // ====================== Constructor: connecting =========================
  bool connectEvent(FBInstance& sourceFBInstance, const char* outEventName,
                    const char* outVariableNames[], FBInstance& destFBInstance,
                    const char* inEventName, const char* inVariableNames[],
                    int sizeofVariables) {
    bool result = true;
    EventOutput* outEvent =
        sourceFBInstance.getFBType().findEventOutputByName(outEventName);
    EventInput* inEvent =
        destFBInstance.getFBType().findEventInputByName(inEventName);
    if (!outEvent) {
      //// printf(ERROR, "It Can't find EventOutput by name %s!\n",
      /// outEventName)
      result = false;
    }
    if (!inEvent) {
      //// printf(ERROR, "It Can't find EventInput by name %s!\n",
      /// inEventName)
      result = false;
    }
    if (!result) {
      return false;
    }

    // Whether the Event Type of A and B match
    unsigned int srcEventType = outEvent->getEventType();
    unsigned int dstEventType = inEvent->getEventType();
    if ((dstEventType != EVENT_ANY) && (dstEventType != srcEventType)) {
      // TODO: printf(WARNING, "EventType of srcEventType(%u) and
      // dstEventType(%u) do not match!\n", srcEventType, dstEventType)
      result = false;
    }
    if (eventOutputIsAlreadyConnected(outEvent)) {
      // TODO: printf(WARNING, "outEvent is already connected!\n")
      result = false;
    }
    if (eventInputIsAlreadyConnected(inEvent)) {
      // TODO: printf(WARNING, "inEvent is already connected!\n")
      result = false;
    }

    // check that each of outVariableNames and each of inVariableNames are
    // match
    for (int i = 0; i < sizeofVariables; i++) {
      OutputVariable* outVariable =
          outEvent->findOutputVariableByName(outVariableNames[i]);
      InputVariable* inVariable =
          inEvent->findInputVariableByName(inVariableNames[i]);

      if (!outVariable) {
        Serial.printf(
            "ERROR: Check: It Can't find OutputVariable by name %s!\n",
            outVariableNames[i]);
        result = false;
      }
      if (!inVariable) {
        Serial.printf("ERROR: Check: It Can't find InputVariable by name %s!\n",
                      inVariableNames[i]);
        result = false;
      }
      // check that outVariable and inVariable are match
      if (result) {
        checkForConnectVariable(*outVariable, *inVariable);
        if (!result) {
          //// printf(HINT, "outVariable(%s) & inVariable(%s) are not
          /// match!\n",
          /// outVariable.getName(), inVariable.getName());
          result = false;
        }
      }
    }
    if (!result) {
      //// printf(HINT, "arguments of connection is error!\n");
      return false;
    }

    EventConnection* eventConnection = new EventConnection(
        &sourceFBInstance, outEvent, &destFBInstance, inEvent);
    eventConnections.push_back(eventConnection);

    // connect inputVariables from outputVariables
    for (int i = 0; i < sizeofVariables; i++) {
      OutputVariable* outVariable =
          outEvent->findOutputVariableByName(outVariableNames[i]);
      InputVariable* inVariable =
          inEvent->findInputVariableByName(inVariableNames[i]);

      if (!outVariable) {
        //// printf(ERROR, "Connect: It Can't find OutputVariable by name
        ///%s!\n", outVariableNames[i])
        continue;  // result = false;
      }
      if (!inVariable) {
        //// printf(ERROR, "Connect: It Can't find InputVariable by name
        ///%s!\n",
        /// inVariableNames[i])
        continue;  // result = false;
      }
      connectVariable(*eventConnection, *outVariable, *inVariable);
    }

    return true;
  }

  // connect InputVariable from OutputVariable
  bool connectVariable(EventConnection& eventConnection,
                       OutputVariable& outData, InputVariable& inData) {
    if (!checkForConnectVariable(outData, inData)) {
      return false;
    }

    DataConnection* dataConnection =
        new DataConnection(&eventConnection, &outData, &inData);
    dataConnections.push_back(dataConnection);
    return true;
  }

  bool eventOutputIsAlreadyConnected(EventOutput* eventOutput) const {
    if (eventOutput == NULL) {
      return false;
    }
    for (std::list<EventConnection*>::const_iterator it =
             eventConnections.begin();
         it != eventConnections.end(); ++it) {
      if ((*it)->getSourceEvent() == eventOutput) {
        return true;
      }
    }
    return false;
  }
  bool eventInputIsAlreadyConnected(EventInput* eventInput) const {
    if (eventInput == NULL) {
      return false;
    }
    for (std::list<EventConnection*>::const_iterator it =
             eventConnections.begin();
         it != eventConnections.end(); ++it) {
      if ((*it)->getDestinationEvent() == eventInput) {
        return true;
      }
    }
    return false;
  }
  bool inputVariableIsAlreadyConnected(InputVariable& inputVariable) const {
    for (std::list<DataConnection*>::const_iterator it =
             dataConnections.begin();
         it != dataConnections.end(); ++it) {
      if ((*it)->destination() == &inputVariable) {
        return true;
      }
    }
    return false;
  }

  // check that outVariable and inVariable are match
  bool checkForConnectVariable(OutputVariable& outData,
                               InputVariable& inData) const {
    if (inputVariableIsAlreadyConnected(inData)) {
      // TODO: printf(WARN, "inData is alreay connected!\n");
      return false;
    }
    // check data type
    unsigned int outNumberOfDataType = outData.getNumberOfDataType();
    unsigned int inNumberOfDataType = inData.getNumberOfDataType();
    bool result =
        DataBox::check4ConnectDataType(outNumberOfDataType, inNumberOfDataType);
    if (!result) {
      // TODO: printf(WARNING, "outNumberOfDataType(%u) and
      // inNumberOfDataType(%u) are not matched!\n", outNumberOfDataType,
      // inNumberOfDataType)
      return false;
    } else {
      return true;
    }
  }

  //////////////////////////////////////////////
  void disconnectFBInstance(FBInstance& fbInstance) {
    fbInstance.getFBType().clearTriggeredEventOutputs();

    for (std::list<EventConnection*>::iterator it = eventConnections.begin();
         it != eventConnections.end();
         /*++it*/) {
      if ((*it)->getSourceFBInstance() == &fbInstance ||
          (*it)->getDestinationFBInstance() == &fbInstance) {
        disconnectEventConnection(**it);
        delete *it;
        it = eventConnections.erase(it);  // it++;
      } else {
        ++it;
      }
    }
  }

  void disconnectEventConnection(EventConnection& eventConnection) {
    for (std::list<DataConnection*>::iterator it = dataConnections.begin();
         it != dataConnections.end();
         /*++it*/) {
      if ((*it)->eventConnection() == &eventConnection) {
        delete *it;
        it = dataConnections.erase(it);  // it++;
      } else {
        ++it;
      }
    }
  }

  //////////////////////////////////////////////
  std::list<FBInstance*> fbInstances;
  std::list<EventConnection*> eventConnections;
  std::list<DataConnection*> dataConnections;
  // TODO: std::List<AdapterConnection> adapterConnections;
};

WOODBLOCK_END_PUBLIC_NAMESPACE
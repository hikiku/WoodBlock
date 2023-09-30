// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <WString.h>  // Arduino
#include <iterator>   //
#include <list>

#include <WoodBlock/Macro.h>
#include <WoodBlock/Namespace.hpp>

#include <WoodBlock/Core/Event.h>
#include <WoodBlock/Core/NamedObject.h>
#include <WoodBlock/Core/Variable.h>

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

class FBNetwork;
class FBInstance;

typedef void (*HandleEventOutputCallback)(FBNetwork& fbNetwork,
                                          FBInstance& fbInstance,
                                          EventOutput& eventOutput);

// TODO: FBType：Plug, Socket, Service. create a FBInstance from CompiledCode
// & Declaraitons.
class FBType : public NamedObject {
 public:
  FBType(const char* name)
      : NamedObject(name),
        eventInputs(),
        eventOutputs(),
        inputVariables(),
        outputVariables(),
        triggeredEventOutputs() {}
  virtual ~FBType() {
    // std::list<EventOutput*> triggeredEventOutputs;
    clearTriggeredEventOutputs();

    // remove member variables
    for (std::list<EventInput*>::iterator it = eventInputs.begin();
         it != eventInputs.end(); ++it) {
      delete *it;
    }
    eventInputs.clear();
    for (std::list<EventOutput*>::iterator it = eventOutputs.begin();
         it != eventOutputs.end(); ++it) {
      delete *it;
    }
    eventOutputs.clear();
    for (std::list<InputVariable*>::iterator it = inputVariables.begin();
         it != inputVariables.end(); ++it) {
      delete *it;
    }
    inputVariables.clear();
    for (std::list<OutputVariable*>::iterator it = outputVariables.begin();
         it != outputVariables.end(); ++it) {
      delete *it;
    }
    outputVariables.clear();
  }

  EventInput* findEventInputByName(const String& nameOfEventInput) {
    for (std::list<EventInput*>::iterator it = eventInputs.begin();
         it != eventInputs.end(); ++it) {
      if ((*it)->getName().equals(nameOfEventInput)) {
        return *it;
      }
    }
    return nullptr;
  }
  EventOutput* findEventOutputByName(const String& nameOfEventOutput) {
    for (std::list<EventOutput*>::iterator it = eventOutputs.begin();
         it != eventOutputs.end(); ++it) {
      if ((*it)->getName().equals(nameOfEventOutput)) {
        return *it;
      }
    }
    return nullptr;
  }
  InputVariable* findInputVariableByName(const String& nameOfInputVariable) {
    for (std::list<InputVariable*>::iterator it = inputVariables.begin();
         it != inputVariables.end(); ++it) {
      if ((*it)->getName().equals(nameOfInputVariable)) {
        return *it;
      }
    }
    return nullptr;
  }
  OutputVariable* findOutputVariableByName(const String& nameOfOutputVariable) {
    for (std::list<OutputVariable*>::iterator it = outputVariables.begin();
         it != outputVariables.end(); ++it) {
      if ((*it)->getName().equals(nameOfOutputVariable)) {
        return *it;
      }
    }
    return nullptr;
  }

  // bool isContainedEventInput(const EventInput& eventInput) const {
  //   for (std::list<EventInput*>::iterator it = eventInputs.begin();
  //        it != eventInputs.end(); ++it) {
  //     EventInput* temp = &(*it);
  //     if (temp == &eventInput) {
  //       return true;
  //     }
  //   }
  //   return false;
  // }
  // bool isContainedEventOutput(const EventOutput& eventOutput) const {
  //   for (std::list<EventOutput*>::iterator it = eventOutputs.begin();
  //        it != eventOutputs.end(); ++it) {
  //     EventOutput* temp = &(*it);
  //     if (temp == &eventOutput) {
  //       return true;
  //     }
  //   }
  //   return false;
  // }
  // bool isContainedInputVariable(const EventInput& eventInput,
  //                               const InputVariable& inputVariable) const {
  //   return isContainedEventInput(eventInput) &&
  //          eventInput.isContainedInputVariable(inputVariable);
  // }
  // bool isContainedOutputVariable(const EventOutput& eventOutput,
  //                                const OutputVariable& outputVariable) const
  //                                {
  //   return isContainedEventOutput(eventOutput) &&
  //          eventOutput.isContainedOutputVariable(outputVariable);
  // }

  // ====================== Constructor: adding =========================
  // eg: SInt
  template <class TDataBox>
  Vi<TDataBox>* addInputVariable(const char* nameOfInputVariable) {
    // push a inVariable to std::list<InputVariable*> inputVariables!
    Vi<TDataBox>* inVariable = new Vi<TDataBox>(nameOfInputVariable);
    if (inVariable) {
      inputVariables.push_back(inVariable);
      return inVariable;
    }
    return nullptr;
  }
  // eg: SInt
  template <class TDataBox>
  Vo<TDataBox>* addOutputVariable(const char* nameOfOutputVariable) {
    // push a outVariable to std::list<OutputVariable*> outputVariables!
    Vo<TDataBox>* outVariable =
        new Vo<TDataBox>(nameOfOutputVariable);  // OutputVariable
    if (outVariable) {
      outputVariables.push_back(outVariable);
      return outVariable;
    }
    return nullptr;
  }

  // eg: EVENT_ANY, ...
  // template <class TEventType>
  EventInput* addEventInput(const char* nameOfEventInput,
                            const char* inputVariableNames[] = nullptr,
                            int sizeofInVariables = 0) {
    EventInput* inEvent = new EventInput(*this, nameOfEventInput);
    if (inEvent) {
      bool result =
          inEvent->addInVariablesByNames(inputVariableNames, sizeofInVariables);
      // add eventInputs ot std::list<EventInput*> eventInputs;
      if (result) {
        eventInputs.push_back(inEvent);
        return inEvent;
      } else {
        // TODO: printf (ERROR, "It fail for calling
        // addInVariablesByNames(EventInput[%s])!", nameOfEventInput);
        delete inEvent;
        return nullptr;
      }
    } else {
      // TODO: printf (ERROR, "It fail for adding EventInput[%s]!",
      // nameOfEventInput);
      return nullptr;
    }
  }
  // eg: EVENT_ANY, ...
  // template <class TEventType>
  EventOutput* addEventOutput(const char* nameOfEventOutput,
                              const char* outputVariableNames[],
                              int sizeofOutVariables) {
    // TODO: ...
    EventOutput* outEvent = new EventOutput(*this, nameOfEventOutput);
    if (outEvent) {
      bool result = outEvent->addOutVariablesByNames(outputVariableNames,
                                                     sizeofOutVariables);
      // add outEvent to std::list<EventOutput*> eventOutputs;
      if (result) {
        eventOutputs.push_back(outEvent);
        return outEvent;
      } else {
        // TODO: printf (ERROR, "It fail for calling
        // addOutVariablesByNames(EventInput[%s])!", nameOfEventOutput);
        delete outEvent;
        return nullptr;
      }
    } else {
      // TODO: printf (ERROR, "It fail for adding addEventOutput[%s]!",
      // nameOfEventOutput);
      return nullptr;
    }
  }

  // ====================== Normal: running =========================
  void processEventInput(FBNetwork& fbNetwork, EventConnection& eventConnect,
                         FBInstance& fbInstance, EventInput& eventInput,
                         SearchOutDataCallback searchOutDataCallback,
                         HandleEventOutputCallback handleEventOutputCallback) {
    EventInput::sample(fbNetwork, eventConnect, fbInstance, eventInput,
                       searchOutDataCallback);
    executeEventInput(eventInput);  // execution ecc
    handleAllOfEventOutputs(fbNetwork, fbInstance, handleEventOutputCallback);
  }

  virtual bool fetchExternalEvents(
      FBNetwork& fbNetwork, FBInstance& fbInstance,
      HandleEventOutputCallback handleEventOutputCallback) = 0;

  void clearTriggeredEventOutputs() {
    triggeredEventOutputs.clear();
  }

  // ====================== Deconstructor: removing =========================
  // Some private functions in ~FBType()

 protected:
  virtual void executeEventInput(const EventInput& inEvent) = 0;
  //{
  // TODO: ......
  // if inEvent.is("INIT") { ... }
  // else if inEvent.is("RESET") { ... }
  // ...... OutData.set(xyz)......
  // ...... generateEventOutput(EventOutput &outEvent)......
  //}

  void generateEventOutput(EventOutput& outEvent) {
    outEvent.generate();
    triggeredEventOutputs.push_back(&outEvent);
  }

  void handleAllOfEventOutputs(
      FBNetwork& fbNetwork, FBInstance& fbInstance,
      HandleEventOutputCallback handleEventOutputCallback) {
    for (std::list<EventOutput*>::iterator it = triggeredEventOutputs.begin();
         it != triggeredEventOutputs.end();) {
      handleEventOutputCallback(fbNetwork, fbInstance, **it);
      it = triggeredEventOutputs.erase(it);  // it++;
    }
  }

 private:
  std::list<EventInput*> eventInputs;    // 0..*
  std::list<EventOutput*> eventOutputs;  // 0..*

  std::list<InputVariable*> inputVariables;    // 0..*
  std::list<OutputVariable*> outputVariables;  // 0..*

  // TODO: std::list<Plug> plugs; // 0..*
  // TODO: std::list<Socket> sockets; // 0..*

  // TODO: std::list<Service> services; // 0..*

  std::list<EventOutput*>
      triggeredEventOutputs;  // pop_front() & push_back(), erase()/clear()
};

class BasicFBType : public FBType {
 public:
  BasicFBType(const char* name) : FBType(name), internalVariables() {}
  virtual ~BasicFBType() {
    for (std::list<InternalVariable*>::iterator it = internalVariables.begin();
         it != internalVariables.end(); ++it) {
      delete *it;
    }
    internalVariables.clear();
  }

  InternalVariable* findInternalVariableByName(
      const String& nameOfInternalVariable) {
    for (std::list<InternalVariable*>::iterator it = internalVariables.begin();
         it != internalVariables.end(); ++it) {
      if ((*it)->getName().equals(nameOfInternalVariable)) {
        return *it;
      }
    }
    return nullptr;
  }

  // eg: SInt
  template <class TDataBox>
  Vt<TDataBox>* addInternalVariable(const char* nameOfInternalVariable) {
    // push a internalVariable to std::list<InternalVariable*>
    // internalVariables!
    Vt<TDataBox>* internalVariable = new Vt<TDataBox>(nameOfInternalVariable);
    if (internalVariable) {
      internalVariables.push_back(internalVariable);
      return internalVariable;
    }
    return nullptr;
  }

  bool fetchExternalEvents(
      FBNetwork& fbNetwork, FBInstance& fbInstance,
      HandleEventOutputCallback handleEventOutputCallback) {
    return true;
  }

 private:
  std::list<InternalVariable*> internalVariables;  // 0..*
};

class CompositeFBType : public FBType {
  // TODO:
 public:
  CompositeFBType(const char* name) : FBType(name) {}

  // TODO: add callback of eventoutputs
  bool fetchExternalEvents(
      FBNetwork& fbNetwork, FBInstance& fbInstance,
      HandleEventOutputCallback handleEventOutputCallback) {
    return true;
  }

 private:
  // TODO: FBNetwork: fbNetwork; // 1..1 // fb_instance_list &&
  // fb_connection_list
};

class SIFBType : public BasicFBType {  // FBType
 public:
  SIFBType(const char* name) : BasicFBType(name) {}  //, siiEvents()
  ~SIFBType() {
    // // std::list<ServiceInterfaceInEvent*> siiEvents;
    // for (std::list<ServiceInterfaceInEvent*>::iterator it =
    // siiEvents.begin(); it != siiEvents.end();)
    // {
    //     delete *it;
    //     it = siiEvents.erase(it); // it++;
    // }
  }

  // TODO: add callback of eventoutputs
  bool fetchExternalEvents(
      FBNetwork& fbNetwork, FBInstance& fbInstance,
      HandleEventOutputCallback handleEventOutputCallback) {
    bool result = captureAndExecuteServiceInterfaceInEvent();  // execution ecc
    if (result) {
      handleAllOfEventOutputs(fbNetwork, fbInstance, handleEventOutputCallback);
    }
    return true;
  }

 protected:
  virtual bool captureAndExecuteServiceInterfaceInEvent() = 0;
  // {
  //     // Get extra/sifb event and/or timer event
  //     ServiceInterfaceInEvent *siiEvent = captureServiceInterfaceInEvent();
  //     // if no extra/sifb event, return false;
  //     if ((siiEvent == nullptr) || (siiEvent == NULL))
  //     {
  //         //// printf (HINT, "It has no ServiceInterfaceInEvent to
  //         capture!\n"]) return false;
  //     }
  //     // virtual ServiceInterfaceInEvent *captureServiceInterfaceInEvent() =
  //     0; executionServiceInterfaceInEvent(*siiEvent);
  //     // virtual void
  //     executionServiceInterfaceInEvent(ServiceInterfaceInEvent &siiEvent) =
  //     0;
  //     // {
  //     //   TODO: ......
  //     //   if siiEvent.is("AAA") { ... }
  //     //   else if siiEvent.is("BBB") { ... }
  //     //   ...... OutData.set(xyz)......
  //     //   ...... generateEventOutput(EventOutput &outEvent)......
  //     //  }
  //     return true;
  // }

 private:
  // std::list<ServiceInterfaceInEvent*> siiEvents;
};

WOODBLOCK_END_PUBLIC_NAMESPACE
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

class FBInstance;

typedef void (*HandleEventOutputCallback)(/*FBNetwork**/ void* fbNetwork,
                                          FBInstance* fbInstance,
                                          EventOutput* eventOutput);

class FBInstance : public NamedObject {
 public:
  FBInstance(const char* name)
      : name(name),
        eventInputs(),
        eventOutputs(),
        inputVariables(),
        outputVariables(),
        triggeredEventOutputs() {}
  virtual ~FBInstance()  // TODO: = 0;
  {
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

  const String& getName() {
    return name;
  }

  EventInput* findInEventByName(const String& inEventName) {
    for (std::list<EventInput*>::iterator it = eventInputs.begin();
         it != eventInputs.end(); ++it) {
      if ((*it)->getName().equals(inEventName)) {
        return *it;
      }
    }
    return nullptr;
  }
  EventOutput* findOutEventByName(const String& outEventName) {
    for (std::list<EventOutput*>::iterator it = eventOutputs.begin();
         it != eventOutputs.end(); ++it) {
      if ((*it)->getName().equals(outEventName)) {
        return *it;
      }
    }
    return nullptr;
  }
  InputVariable* findInVariableByName(const String& inVariableName) {
    for (std::list<InputVariable*>::iterator it = inputVariables.begin();
         it != inputVariables.end(); ++it) {
      if ((*it)->getName().equals(inVariableName)) {
        return *it;
      }
    }
    return nullptr;
  }
  OutputVariable* findOutVariableByName(const String& outVariableName) {
    for (std::list<OutputVariable*>::iterator it = outputVariables.begin();
         it != outputVariables.end(); ++it) {
      if ((*it)->getName().equals(outVariableName)) {
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
  Vi<TDataBox>* addInVariable(const char* name) {
    // push a inVariable to std::list<InputVariable*> inputVariables!
    Vi<TDataBox>* inVariable = new Vi<TDataBox>(name);
    if (inVariable) {
      inputVariables.push_back(inVariable);
      return inVariable;
    }
    return nullptr;
  }
  // eg: SInt
  template <class TDataBox>
  Vo<TDataBox>* addOutVariable(const char* name) {
    // push a outVariable to std::list<OutputVariable*> outputVariables!
    Vo<TDataBox>* outVariable = new Vo<TDataBox>(name);  // OutputVariable
    if (outVariable) {
      outputVariables.push_back(outVariable);
      return outVariable;
    }
    return nullptr;
  }

  // eg: EVENT_ANY, ...
  // template <class TEventType>
  EventInput* addInEvent(const char* name, const char* inVariableNames[],
                         int sizeofInVariables) {
    EventInput* inEvent = new EventInput(*this, name);
    if (inEvent) {
      bool result =
          inEvent->addInVariablesByNames(inVariableNames, sizeofInVariables);
      // add eventInputs ot std::list<EventInput*> eventInputs;
      if (result) {
        eventInputs.push_back(inEvent);
        return inEvent;
      } else {
        // TODO: printf (ERROR, "It fail for calling
        // addInVariablesByNames(EventInput[%s])!", name);
        delete inEvent;
        return nullptr;
      }
    } else {
      // TODO: printf (ERROR, "It fail for adding EventInput[%s]!", name);
      return nullptr;
    }
  }
  // eg: EVENT_ANY, ...
  // template <class TEventType>
  EventOutput* addOutEvent(const char* name, const char* outVariableNames[],
                           int sizeofOutVariables) {
    // TODO: ...
    EventOutput* outEvent = new EventOutput(*this, name);
    if (outEvent) {
      bool result = outEvent->addOutVariablesByNames(outVariableNames,
                                                     sizeofOutVariables);
      // add outEvent to std::list<EventOutput*> eventOutputs;
      if (result) {
        eventOutputs.push_back(outEvent);
        return outEvent;
      } else {
        // TODO: printf (ERROR, "It fail for calling
        // addOutVariablesByNames(EventInput[%s])!", name);
        delete outEvent;
        return nullptr;
      }
    } else {
      // TODO: printf (ERROR, "It fail for adding addOutEvent[%s]!", name);
      return nullptr;
    }
  }

  // ====================== Normal: running =========================
  void processEventInput(/*FBNetwork**/ void* fbNetwork,
                         /*EventConnection**/ void* eventConnect,
                         EventInput* eventInput,
                         SearchOutDataCallback searchOutDataCallback,
                         HandleEventOutputCallback handleEventOutputCallback) {
    EventInput::sample(fbNetwork, eventConnect, eventInput, searchOutDataCallback);
    executeEventInput(*eventInput);  // execution ecc
    handleAllOfEventOutputs(fbNetwork, handleEventOutputCallback);
  }

  void clearTriggeredEventOutputs() {
    triggeredEventOutputs.clear();
  }

  // ====================== Deconstructor: removing =========================
  // Some private functions in ~FBInstance()

 protected:
  virtual void executeEventInput(EventInput& inEvent) = 0;
  //{
  // TODO: ......
  // if inEvent.is("INIT") { ... }
  // else if inEvent.is("RESET") { ... }
  // ...... OutData.set(xyz)......
  // ...... generateOutEvent(EventOutput &outEvent)......
  //}

  void generateOutEvent(EventOutput& outEvent) {
    outEvent.generate();
    triggeredEventOutputs.push_back(&outEvent);
  }

  void handleAllOfEventOutputs(
      /*FBNetwork**/ void* fbNetwork,
      HandleEventOutputCallback handleEventOutputCallback) {
    for (std::list<EventOutput*>::iterator it = triggeredEventOutputs.begin();
         it != triggeredEventOutputs.end();) {
      handleEventOutputCallback(fbNetwork, this, *it);
      it = triggeredEventOutputs.erase(it);  // it++;
    }
  }

 private:
  String name;

  std::list<EventInput*> eventInputs;
  std::list<EventOutput*> eventOutputs;

  std::list<InputVariable*> inputVariables;
  std::list<OutputVariable*> outputVariables;
  // TODO: std::list<InternalVariable*> internalVariables;

  std::list<EventOutput*>
      triggeredEventOutputs;  // pop_front() & push_back(), erase()/clear()
};

class ServiceInterfaceBlock : public FBInstance {
 public:
  ServiceInterfaceBlock(const char* name) : FBInstance(name) {}  //, siiEvents()
  ~ServiceInterfaceBlock() {
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
      /*FBNetwork**/ void* fbNetwork,
      HandleEventOutputCallback handleEventOutputCallback) {
    bool result = captureAndExecuteServiceInterfaceInEvent();  // execution ecc
    if (result) {
      handleAllOfEventOutputs(fbNetwork, handleEventOutputCallback);
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
  //     //   ...... generateOutEvent(EventOutput &outEvent)......
  //     //  }
  //     return true;
  // }

 private:
  // std::list<ServiceInterfaceInEvent*> siiEvents;
};

WOODBLOCK_END_PUBLIC_NAMESPACE
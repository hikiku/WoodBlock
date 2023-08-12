// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <WString.h>  // Arduino
#include <iterator>   //
#include <list>

#include <WoodBlock/Namespace.hpp>
#include <WoodBlock/Macro.h>

#include <WoodBlock/Core/Event.h>
#include <WoodBlock/Core/Variable.h>

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

// TODO: FBInstance?
class FBInstance {
 public:
  FBInstance(const char* name)
      : name(name),
        eventInputs(),
        eventOutputs(),
        inputVariables(),
        outputVariables(),
        triggeredOutEvents() {}
  virtual ~FBInstance()  // TODO: = 0;
  {
    // std::list<EventOutput*> triggeredOutEvents;
    triggeredOutEvents.clear();

    // clear some connects of eventInputs & eventOutputs, inputVariables & outputVariables
    disconnect();

    // remove member variables
    // std::list<EventInput*> eventInputs;
    for (std::list<EventInput*>::iterator it = eventInputs.begin();
         it != eventInputs.end(); ++it) {
      delete *it;
    }
    eventInputs.clear();
    // std::list<EventOutput*> eventOutputs;
    for (std::list<EventOutput*>::iterator it = eventOutputs.begin();
         it != eventOutputs.end(); ++it) {
      delete *it;
    }
    eventOutputs.clear();
    // std::list<InputVariable*> inputVariables;
    for (std::list<InputVariable*>::iterator it = inputVariables.begin();
         it != inputVariables.end(); ++it) {
      delete *it;
    }
    inputVariables.clear();
    // std::list<OutputVariable*> outputVariables;
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
    // std::list<EventInput*> eventInputs;
    for (std::list<EventInput*>::iterator it = eventInputs.begin();
         it != eventInputs.end(); ++it) {
      if ((*it)->getName().equals(inEventName)) {
        return *it;
      }
    }

    return nullptr;
  }
  EventOutput* findOutEventByName(const String& outEventName) {
    // std::list<EventOutput*> eventOutputs;
    for (std::list<EventOutput*>::iterator it = eventOutputs.begin();
         it != eventOutputs.end(); ++it) {
      if ((*it)->getName().equals(outEventName)) {
        return *it;
      }
    }

    return nullptr;
  }
  InputVariable* findInVariableByName(const String& inVariableName) {
    // std::list<InputVariable*> inputVariables;
    for (std::list<InputVariable*>::iterator it = inputVariables.begin();
         it != inputVariables.end(); ++it) {
      if ((*it)->getName().equals(inVariableName)) {
        return *it;
      }
    }

    return nullptr;
  }
  OutputVariable* findOutVariableByName(const String& outVariableName) {
    // std::list<OutputVariable*> outputVariables;
    for (std::list<OutputVariable*>::iterator it = outputVariables.begin();
         it != outputVariables.end(); ++it) {
      if ((*it)->getName().equals(outVariableName)) {
        return *it;
      }
    }

    return nullptr;
  }

  // ====================== Constructor: adding =========================
  // eg: SInt
  template <class TDataBox>
  Vi<TDataBox>* addInVariable(const char* name) {
    // push a inVariable to std::list<InputVariable*> inputVariables!
    Vi<TDataBox>* inVariable =
        new Vi<TDataBox>(name);
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
    Vo<TDataBox>* outVariable =
        new Vo<TDataBox>(name);  // OutputVariable
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

  // ====================== Constructor: connecting =========================
  bool connectTo(const char* outEventName, const char* outVariableNames[],
                 FBInstance& destFunctionBlock, const char* inEventName,
                 const char* inVariableNames[], int sizeofVariables) {
    bool result = true;
    EventOutput* outEvent = findOutEventByName(outEventName);
    EventInput* inEvent = destFunctionBlock.findInEventByName(inEventName);
    if (!outEvent) {
      //// printf(ERROR, "It Can't find EventOutput by name %s!\n",
      ///outEventName)
      result = false;
    }
    if (!inEvent) {
      //// printf(ERROR, "It Can't find EventInput by name %s!\n", inEventName)
      result = false;
    }
    if (!result) {
      return false;
    }

    return outEvent->connectTo(outVariableNames, *inEvent, inVariableNames,
                               sizeofVariables);
  }

  // ====================== Normal: running =========================
  void processInEvent(EventInput& inEvent) {
    inEvent.sample();                  // sample input variables
    executeInEvent(inEvent);           // execution ecc
    dispatchAndExecuteAllOutEvents();  // dispatch and execute all output events
  }

  // ====================== Deconstructor: disconnecting
  // =========================
  void disconnect() {
    // std::list<EventOutput*> triggeredOutEvents;
    triggeredOutEvents.clear();

    // std::list<EventInput*> eventInputs;
    for (std::list<EventInput*>::iterator it = eventInputs.begin();
         it != eventInputs.end(); ++it) {
      (*it)->disconnect();
    }

    // std::list<EventOutput*> eventOutputs;
    for (std::list<EventOutput*>::iterator it = eventOutputs.begin();
         it != eventOutputs.end(); ++it) {
      (*it)->disconnect();
    }
  }

  // ====================== Deconstructor: removing =========================
  // Some private functions in ~FBInstance()

 protected:
  virtual void executeInEvent(EventInput& inEvent) = 0;
  //{
  // TODO: ......
  // if inEvent.is("INIT") { ... }
  // else if inEvent.is("RESET") { ... }
  // ...... OutData.set(xyz)......
  // ...... generateOutEvent(EventOutput &outEvent)......
  //}

  void generateOutEvent(EventOutput& outEvent) {
    outEvent.generate();
    triggeredOutEvents.push_back(&outEvent);
  }

  // dispatch and execute all output events
  void dispatchAndExecuteAllOutEvents() {
    for (std::list<EventOutput*>::iterator it = triggeredOutEvents.begin();
         it != triggeredOutEvents.end();) {
      (*it)->dispatchAndExecute();
      it = triggeredOutEvents.erase(it);  // it++;
    }
    // triggeredOutEvents.clear();
  }

 private:
  void disconnectInEvent(const String& inEventName) {
    EventInput* inEvent = findInEventByName(inEventName);
    if (inEvent) {
      inEvent->disconnect();
    }
  }
  void disconnectOutEvent(const String& outEventName) {
    EventOutput* outEvent = findOutEventByName(outEventName);
    if (outEvent) {
      outEvent->disconnect();
    }
  }

  String name;

  std::list<EventInput*> eventInputs;
  std::list<EventOutput*> eventOutputs;

  std::list<InputVariable*> inputVariables;
  std::list<OutputVariable*> outputVariables;
  // TODO: std::list<InternalVariable*> internalVariables;

  std::list<EventOutput*>
      triggeredOutEvents;  // pop_front() & push_back(), erase()/clear()
};

class ServiceInterfaceBlock : public FBInstance {
 public:
  ServiceInterfaceBlock(const char* name)
      : FBInstance(name) {}  //, siiEvents()
  ~ServiceInterfaceBlock() {
    // // std::list<ServiceInterfaceInEvent*> siiEvents;
    // for (std::list<ServiceInterfaceInEvent*>::iterator it =
    // siiEvents.begin(); it != siiEvents.end();)
    // {
    //     delete *it;
    //     it = siiEvents.erase(it); // it++;
    // }
  }

  bool fetchExternalEvents() {
    bool result = captureAndExecuteServiceInterfaceInEvent();  // execution ecc
    if (result) {
      dispatchAndExecuteAllOutEvents();  // dispatch and execute all output
                                         // events
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
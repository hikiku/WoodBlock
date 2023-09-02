// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <Arduino.h>  // Arduino
#include <WString.h>  // Arduino
#include <iterator>   //
#include <list>

#include <WoodBlock/Macro.h>
#include <WoodBlock/Namespace.hpp>

#include <WoodBlock/Core/Variable.h>

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

class FBType;
class EventInput;
class EventOutput;

// Event Type
#define EVENT_ANY (0)
#define EVENT_USER_EXTENDED_BASE (10000)

class FBNetwork;
class EventInput;
class EventConnection;

typedef OutputVariable* (*SearchOutDataCallback)(
    FBNetwork& fbNetwork, EventConnection& eventConnect,
    EventInput& eventInput, InputVariable& inputVariable);

class Event {
 public:
  Event(FBType& owner, const String& name,
        unsigned int eventType = EVENT_ANY)
      : owner(owner), name(name), eventType(eventType) {}
  virtual ~Event() {}  // TODO: = 0;
  const String& getName() const {
    return name;
  }
  unsigned int getEventType() const {
    return eventType;
  }

 protected:
  FBType& getFBInstance() {
    return owner;
  }

 private:
  FBType& owner;  // belongs to
  String name;
  unsigned int eventType;
};

class EventInput : public Event {
 public:
  EventInput(FBType& owner, const char* name)
      : Event(owner, name), inputVariables() {}  //, outEvent(nullptr)
  ~EventInput() {}

  InputVariable* findInVariableByName(const char* inVariableName) {
    if (inVariableName == nullptr) {
      // printf (DEBUG, "inVariableName is NULL!");
      return nullptr;
    }

    String name(inVariableName);
    for (std::list<InputVariable*>::iterator it = inputVariables.begin();
         it != inputVariables.end(); ++it) {
      if ((*it)->getName().equals(name)) {
        return *it;
      }
    }

    return nullptr;
  }
  // bool isContainedInputVariable(const InputVariable& inputVariable) const {
  //   for (std::list<InputVariable*>::iterator it = inputVariables.begin();
  //        it != inputVariables.end(); ++it) {
  //     InputVariable* temp = &(*it);
  //     if (temp == &inputVariable) {
  //       return true;
  //     }
  //   }
  //   return false;
  // }

  bool addInVariableByName(const char* inVariableName);

  bool addInVariablesByNames(const char* inVariableNames[],
                             int sizeofInVariables) {
    bool result =
        check4AddingInVariablesByNames(inVariableNames, sizeofInVariables);
    if (!result) {
      return false;
    }

    // add
    for (int i = 0; i < sizeofInVariables; i++) {
      bool result = addInVariableByName(inVariableNames[i]);
      if (!result) {
        // TODO: printf (ERROR, "inVariableNames[%d] is error!", %d);
        return false;
      }
    }
    return true;
  }

  // void trigger();

  void _sample(FBNetwork& fbNetwork,
               EventConnection& eventConnect,
               SearchOutDataCallback searchOutDataCallback) {
    for (std::list<InputVariable*>::iterator it = inputVariables.begin();
         it != inputVariables.end(); ++it) {
      OutputVariable* outputVariable =
          searchOutDataCallback(fbNetwork, eventConnect, *this, **it);
      // (*it)->sample(*outputVariable);  //**it = outputVariable
      (*it)->getDataBox() = outputVariable->getDataBox();
    }
  }

  static void sample(FBNetwork& fbNetwork,
                     EventConnection& eventConnect,
                     EventInput& eventInput,
                     SearchOutDataCallback searchOutDataCallback) {
    eventInput._sample(fbNetwork, eventConnect, searchOutDataCallback);
  }

 private:
  bool check4AddingInVariableByName(const char* inVariableName);

  bool check4AddingInVariablesByNames(const char* inVariableNames[],
                                      int sizeofInVariables) {
    if (!inVariableNames) {
      // TODO: printf (ERROR, "inVariableNames is NULL!");
      return false;
    }
    if (sizeofInVariables <= 0) {
      // TODO: printf (ERROR, "sizeofInVariables(%d) is <= 0!",
      // sizeofInVariables);
      return false;
    }

    // check
    for (int i = 0; i < sizeofInVariables; i++) {
      bool result = check4AddingInVariableByName(inVariableNames[i]);
      if (!result) {
        // TODO: printf (ERROR, "inVariableNames[%d] is error!", %d);
        return false;
      }
    }
    return true;
  }

  std::list<InputVariable*> inputVariables;
  // EventOutput* outEvent;
};

class EventOutput : public Event {
 public:
  EventOutput(FBType& owner, const char* name)
      : Event(owner, name), outputVariables() {}  //, inEvent(nullptr)
  ~EventOutput() {
    outputVariables.clear();
  }

  bool addOutVariableByName(const char* outVariableName);

  bool addOutVariablesByNames(const char* outVariableNames[],
                              int sizeofOutVariables) {
    bool result =
        check4AddingOutVariablesByNames(outVariableNames, sizeofOutVariables);
    if (!result) {
      return false;
    }

    // add
    for (int i = 0; i < sizeofOutVariables; i++) {
      bool result = addOutVariableByName(outVariableNames[i]);
      if (!result) {
        // TODO: printf (ERROR, "outputVariables[%d] is error!", %d);
        return false;
      }
    }
    return true;
  }

  OutputVariable* findOutVariableByName(const char* outVariableName) {
    if (outVariableName == nullptr) {
      // printf (DEBUG, "outVariableName is NULL!");
      return nullptr;
    }

    String name(outVariableName);
    for (std::list<OutputVariable*>::iterator it = outputVariables.begin();
         it != outputVariables.end(); ++it) {
      if ((*it)->getName().equals(name)) {
        return *it;
      }
    }
    return nullptr;
  }

  // bool isContainedOutputVariable(const OutputVariable& outputVariable) const
  // {
  //   for (std::list<OutputVariable*>::iterator it = outputVariables.begin();
  //        it != outputVariables.end(); ++it) {
  //     OutputVariable* temp = &(*it);
  //     if (temp == &outputVariable) {
  //       return true;
  //     }
  //   }
  //   return false;
  // }

  void generate() {
    generated = true;
  }
  bool isGenerated() const {
    return generated;
  }

  // bool dispatchAndExecute() {
  //   // TODO:
  //   if (!isGenerated()) {
  //     return false;
  //   }
  //   if (!inEvent) {
  //     return false;
  //   }
  //   inEvent->trigger();
  //   clear();
  //   return true;
  // }

 private:
  bool check4AddingOutVariableByName(const char* outVariableName);

  bool check4AddingOutVariablesByNames(const char* outVariableNames[],
                                       int sizeofOutVariables) {
    if (!outVariableNames) {
      // TODO: printf (ERROR, "outVariableNames is NULL!");
      return false;
    }
    if (sizeofOutVariables <= 0) {
      // TODO: printf (ERROR, "sizeofOutVariables(%d) is <= 0!",
      // sizeofOutVariables);
      return false;
    }

    // check
    for (int i = 0; i < sizeofOutVariables; i++) {
      bool result = check4AddingOutVariableByName(outVariableNames[i]);
      if (!result) {
        // TODO: printf (ERROR, "outVariableNames[%d] is error!", %d);
        return false;
      }
    }
    return true;
  }

  void clear() {
    generated = false;
  }

  std::list<OutputVariable*> outputVariables;
  // EventInput* inEvent;
  bool generated;  // Has a out event been alreay generated?
};

class ServiceInterfaceInEvent : public EventInput {};

WOODBLOCK_END_PUBLIC_NAMESPACE
// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <Arduino.h>  // Arduino
#include <WString.h>  // Arduino
#include <iterator>   //
#include <list>

#include <WoodBlock/Namespace.hpp>
#include <WoodBlock/Macro.h>

#include <WoodBlock/Core/Variable.h>

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

class FunctionBlock;
class EventInput;
class EventOutput;

// Event Type
#define EVENT_ANY (0)
#define EVENT_USER_EXTENDED_BASE (10000)

class Event {
 public:
  Event(FunctionBlock& owner, const String& name,
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
  FunctionBlock& getOwner() {
    return owner;
  }

 private:
  FunctionBlock& owner;  // belongs to
  String name;
  unsigned int eventType;
};

class EventInput : public Event {
 public:
  EventInput(FunctionBlock& owner, const char* name)
      : Event(owner, name), inputVariables(), outEvent(nullptr) {
    //
  }
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

  bool isAlreadyConnected() const {
    return (outEvent == nullptr) ? false : true;
  }
  bool setConnectSource(EventOutput& outEvent)  // friend class method
  {
    if (this->outEvent == nullptr) {
      this->outEvent = &outEvent;
      return true;
    }
    return false;
  }
  void disconnect();

  void trigger();

  void sample() {
    // sample all of inputVariables
    for (std::list<InputVariable*>::iterator it = inputVariables.begin();
         it != inputVariables.end(); ++it) {
      (*it)->sample();
    }
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
  EventOutput* outEvent;
};

class EventOutput : public Event {
 public:
  EventOutput(FunctionBlock& owner, const char* name)
      : Event(owner, name), outputVariables(), inEvent(nullptr) {}
  ~EventOutput() {
    // std::list<OutputVariable*> outputVariables;
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

  bool isAlreadyConnected() const {
    return (inEvent == nullptr) ? false : true;
  }
  bool connectTo(const char* outVariableNames[], EventInput& inEvent,
                 const char* inVariableNames[], int sizeofVariables) {
    bool result = true;

    // Whether the Event Type of A and B match
    unsigned int srcEventType = getEventType();
    unsigned int dstEventType = inEvent.getEventType();
    if ((dstEventType != EVENT_ANY) && (dstEventType != srcEventType)) {
      // TODO: printf(WARNING, "EventType of srcEventType(%u) and
      // dstEventType(%u) do not match!\n", srcEventType, dstEventType)
      result = false;
    }
    if (isAlreadyConnected()) {
      // TODO: printf(WARNING, "outEvent is already connected!\n")
      result = false;
    }
    if (inEvent.isAlreadyConnected()) {
      // TODO: printf(WARNING, "inEvent is already connected!\n")
      result = false;
    }

    // check that each of outVariableNames and each of inVariableNames are match
    for (int i = 0; i < sizeofVariables; i++) {
      OutputVariable* outVariable = findOutVariableByName(outVariableNames[i]);
      InputVariable* inVariable =
          inEvent.findInVariableByName(inVariableNames[i]);

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
        inVariable->checkForConnectFrom(*outVariable);
      }
    }
    if (!result) {
      //// printf(HINT, "arguments of connection is error!\n");
      return false;
    }

    // connect inputVariables from outputVariables
    for (int i = 0; i < sizeofVariables; i++) {
      OutputVariable* outVariable = findOutVariableByName(outVariableNames[i]);
      InputVariable* inVariable =
          inEvent.findInVariableByName(inVariableNames[i]);

      if (!outVariable) {
        //// printf(ERROR, "Connect: It Can't find OutputVariable by name
        ///%s!\n", outVariableNames[i])
        continue;  // result = false;
      }
      if (!inVariable) {
        //// printf(ERROR, "Connect: It Can't find InputVariable by name %s!\n",
        /// inVariableNames[i])
        continue;  // result = false;
      }
      inVariable->connectFrom(*outVariable);
    }

    // connect outEvent to inEvent
    this->inEvent = &inEvent;
    inEvent.setConnectSource(*this);
    return true;
  }

  void disconnect() {
    if (inEvent) {
      inEvent->disconnect();
    }
  }
  // friend class method
  void clearConnectDestination() {
    if (inEvent) {
      inEvent = nullptr;
    }
  }

  void generate() {
    generated = true;
  }
  bool isGenerated() const {
    return generated;
  }

  bool dispatchAndExecute() {
    // TODO:
    if (!isGenerated()) {
      return false;
    }
    if (!inEvent) {
      return false;
    }
    inEvent->trigger();
    clear();
    return true;
  }

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
  EventInput* inEvent;  // event observer, to event, connect to
  bool generated;       // Has a out event been alreay generated?
};

class ServiceInterfaceInEvent : public EventInput {
  //
};

WOODBLOCK_END_PUBLIC_NAMESPACE
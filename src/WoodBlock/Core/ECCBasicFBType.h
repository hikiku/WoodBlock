// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <Arduino.h>  // Arduino
// #include <WString.h>  // Arduino
// #include <iterator>   //
// #include <list>

#include <WoodBlock/Macro.h>
#include <WoodBlock/Namespace.hpp>

#include <WoodBlock/Core/FBType.h>
#include <WoodBlock/Core/NamedObject.h>

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

class ECCBasicFBType;

class ECState;
class ECAction;
class Algorithm;
class ECTransition;

typedef EventInput ECTransitionEvent;
typedef bool (*GuardCondition)(const ECCBasicFBType& eccBasicFBType);

class ECState : public NamedObject {
 public:
  ECState(const String& name, ECCBasicFBType& eccBasicFBType)
      : NamedObject(name), eccBasicFBType(eccBasicFBType), ecActions() {}
  ~ECState();

  void addECAction(Algorithm* algorithm, EventOutput* eventOutput);
  void executeECActions();

 private:
  // ECState(const String& name) {}

  ECCBasicFBType& eccBasicFBType;
  std::list<ECAction*> ecActions;  // length can be zero
};

// NOTE: merge it to ECState!
class ECAction {
 public:
  ECAction(ECCBasicFBType& eccBasicFBType, Algorithm* algorithm,
           EventOutput* eventOutput)
      : eccBasicFBType(eccBasicFBType),
        algorithm(algorithm),
        eventOutput(eventOutput) {}

  void execute();

 private:
  // ECAction() {}

  ECCBasicFBType& eccBasicFBType;
  Algorithm* algorithm;      // can be NULL
  EventOutput* eventOutput;  // can be NULL
};

typedef void (*AlgorithmCallback)(); /*BasicFBType& basicFBType*/

class Algorithm : public NamedObject {
 public:
  Algorithm(const String& name, ECCBasicFBType& eccBasicFBType,
            AlgorithmCallback algorithmCallback)
      : NamedObject(name),
        eccBasicFBType(eccBasicFBType),
        algorithmCallback(algorithmCallback) {}

  void execute() {
    algorithmCallback();  // eccBasicFBType->algorithmCallback()???
  }

 protected:
  template <typename T>
  const T& vi(const String& nameOfInputVariable);

  template <typename T>
  T& vo(const String& nameOfOutputVariable);

  template <typename T>
  T& vt(const String& nameOfInternalVariable);

 private:
  // Algorithm() {}

  ECCBasicFBType& eccBasicFBType;
  AlgorithmCallback algorithmCallback;
};

// The transition between states can be depicted by either
// a) an event,
// b) an event with a Boolean expression,
// c) a Boolean expression and no event or
// d) no event and be unconditional (i.e. always true).
class ECTransitionCondition {
 public:
  ECTransitionCondition(const ECCBasicFBType& eccBasicFBType,
                        const ECTransitionEvent* ecTransitionEvent = nullptr,
                        const GuardCondition guardCondition = nullptr,
                        bool alwaysTrue = false)
      : eccBasicFBType(eccBasicFBType),
        ecTransitionEvent(ecTransitionEvent),
        guardCondition(guardCondition),
        alwaysTrue(alwaysTrue) {}

  bool checkECTransitionEvent(
      const ECTransitionEvent& ecTransitionEvent_) const {
    if (this->ecTransitionEvent == nullptr) {
      return false;
    }
    if (&ecTransitionEvent_ != this->ecTransitionEvent) {
      return false;
    }

    if (guardCondition) {
      return guardCondition(eccBasicFBType);
    }

    return alwaysTrue;
  }
  bool checkECTransitionEvent() const {
    if (this->ecTransitionEvent != nullptr) {
      return false;
    }

    if (guardCondition) {
      return guardCondition(eccBasicFBType);
    }

    return alwaysTrue;
  }

 private:
  // ECTransitionCondition(const ECCBasicFBType& eccBasicFBType) {}

  const ECCBasicFBType& eccBasicFBType;
  const ECTransitionEvent* ecTransitionEvent;
  const GuardCondition guardCondition;
  bool alwaysTrue;
};

class ECTransition {
 public:
  ECTransition(const ECCBasicFBType& eccBasicFBType, ECState& sourceECState,
               ECState& destinationECState,
               const ECTransitionEvent* ecTransitionEvent = nullptr,
               const GuardCondition guardCondition = nullptr,
               bool alwaysTrue = false)
      : eccBasicFBType(eccBasicFBType),
        sourceECState(sourceECState),
        destinationECState(destinationECState),
        ecTransitionCondition(eccBasicFBType, ecTransitionEvent, guardCondition,
                              alwaysTrue) {}

  bool evaluateTransitionWithEventInput(const ECState& currentECState,
                                        const EventInput& eventInput) const {
    if (&currentECState != &sourceECState) {
      return false;
    }
    return ecTransitionCondition.checkECTransitionEvent(eventInput);
  }
  bool evaluateTransitionWithoutEventInput(
      const ECState& currentECState) const {
    if (&currentECState != &sourceECState) {
      return false;
    }
    return ecTransitionCondition.checkECTransitionEvent();
  }

  inline ECState& getDestinationECState() const {
    return destinationECState;
  }

 private:
  // ECTransition(const ECCBasicFBType& eccBasicFBType ...) {}

  const ECCBasicFBType& eccBasicFBType;
  ECState& sourceECState;
  ECState& destinationECState;
  ECTransitionCondition ecTransitionCondition;
};

class ECCBasicFBType : public BasicFBType {
 public:
  ECCBasicFBType(const char* name)
      : BasicFBType(name),
        currentECState(),
        ecStates(),
        ecTransitions(),
        algorithms() {}

  virtual ~ECCBasicFBType() {
    for (std::list<ECState*>::iterator it = ecStates.begin();
         it != ecStates.end(); it++) {
      delete *it;
    }
    ecStates.clear();

    for (std::list<ECTransition*>::iterator it = ecTransitions.begin();
         it != ecTransitions.end(); it++) {
      delete *it;
    }
    ecTransitions.clear();

    for (std::list<Algorithm*>::iterator it = algorithms.begin();
         it != algorithms.end(); it++) {
      delete *it;
    }
    algorithms.clear();
  }

  Algorithm* addAlgorithm(const String& nameOfAlgorithm,
                          AlgorithmCallback algorithmCallback) {
    Algorithm* algorithm =
        new Algorithm(nameOfAlgorithm, *this, algorithmCallback);
    algorithms.push_back(algorithm);
    return algorithm;
  }

  ECState* addECState(const String& nameOfECState) {
    ECState* ecState = new ECState(nameOfECState, *this);
    if (currentECState == nullptr) {
      currentECState = ecState;
    }
    return ecState;
  }

  static void addECAction(ECState& ecState, Algorithm* algorithm,
                          EventOutput* eventOutput) {
    ecState.addECAction(algorithm, eventOutput);
  }

  void addECTransition(ECState& sourceECState, ECState& destinationECState,
                       const ECTransitionEvent* ecTransitionEvent = nullptr,
                       const GuardCondition guardCondition = nullptr,
                       bool alwaysTrue = false) {
    if (ecTransitionEvent == nullptr && guardCondition == nullptr &&
        alwaysTrue == false) {
      // TODO: print(ERROR, "only one is nullptr");
      return;
    }

    ECTransition* ecTransition =
        new ECTransition(*this, sourceECState, destinationECState,
                         ecTransitionEvent, guardCondition, alwaysTrue);
    ecTransitions.push_back(ecTransition);
  }

  void executeEventInput(const EventInput& eventInput) {
    // s1
    ECTransition* ecTransition = evaluateTransitionWithEventInput(eventInput);
    while (ecTransition) {  // && ecTransition->getDestinationECState()
      // s2
      ECState& destinationECState = ecTransition->getDestinationECState();
      currentECState = &destinationECState;  // Transition Crossed
      currentECState->executeECActions();
      ecTransition = evaluateTransitionWithoutEventInput();
    }
    // s0
  }

  ECState* findECStateByName(const String& nameOfECState) {
    for (std::list<ECState*>::iterator it = ecStates.begin();
         it != ecStates.end(); it++) {
      if (strcmp((*it)->getName().c_str(), nameOfECState.c_str()) == 0) {
        return *it;
      }
    }
    return nullptr;
  }
  Algorithm* findAlgorithmByName(const String& nameOfAlgorithm) {
    for (std::list<Algorithm*>::iterator it = algorithms.begin();
         it != algorithms.end(); it++) {
      if (strcmp((*it)->getName().c_str(), nameOfAlgorithm.c_str()) == 0) {
        return *it;
      }
    }
    return nullptr;
  }

 private:
  // ECCBasicFBType(const char *) {}

  void generateEventOutput(EventOutput& outEvent) {
    FBType::generateEventOutput(outEvent);
  }

  ECTransition* evaluateTransitionWithEventInput(const EventInput& eventInput) {
    for (std::list<ECTransition*>::iterator it = ecTransitions.begin();
         it != ecTransitions.end(); it++) {
      if ((*it)->evaluateTransitionWithEventInput(*currentECState,
                                                  eventInput)) {
        return *it;
      }
    }
    return nullptr;
  }
  ECTransition* evaluateTransitionWithoutEventInput() {
    for (std::list<ECTransition*>::iterator it = ecTransitions.begin();
         it != ecTransitions.end(); it++) {
      if ((*it)->evaluateTransitionWithoutEventInput(*currentECState)) {
        return *it;
      }
    }
    return nullptr;
  }

  // ECC： Execuion Control Chart
  ECState* currentECState;
  std::list<ECState*> ecStates;            // length greater than 1
  std::list<ECTransition*> ecTransitions;  // length greater than 1

  std::list<Algorithm*> algorithms;  // 0..*

  friend class ECAction; // ECCBasicFBType::generateEventOutput()
};

WOODBLOCK_END_PUBLIC_NAMESPACE
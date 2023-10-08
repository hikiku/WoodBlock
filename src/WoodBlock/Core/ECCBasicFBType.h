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

#define ECTransitionEvent EventInput

typedef std::function<bool()>
    GuardCondition; /*const ECCBasicFBType& eccBasicFBType*/
// typedef bool (*GuardCondition)(const ECCBasicFBType& eccBasicFBType);

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

typedef std::function<void()> AlgorithmCallback; /*BasicFBType& basicFBType*/
// typedef void (*AlgorithmCallback)(); /*BasicFBType& basicFBType*/

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
      // WB_LOGV("this->ecTransitionEvent == nullptr");
      return false;
    }
    if (&ecTransitionEvent_ != this->ecTransitionEvent) {
      // WB_LOGV("&ecTransitionEvent_ != this->ecTransitionEvent");
      return false;
    }

    if (guardCondition) {
      return guardCondition();  // eccBasicFBType
    }

    // return alwaysTrue;
    return true;
  }
  bool checkECTransitionEvent() const {
    if (this->ecTransitionEvent != nullptr) {
      return false;
    }

    if (guardCondition) {
      return guardCondition();  // eccBasicFBType
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
      // WB_LOGV(
      //     "ECTransition::evaluateTransitionWithEventInput(currentECState=%s, "
      //     "eventInput=%s, sourceECState=%s, ), result= %s",
      //     currentECState.getName().c_str(), eventInput.getName().c_str(),
      //     sourceECState.getName().c_str(), "false");
      return false;
    }

    bool result = ecTransitionCondition.checkECTransitionEvent(eventInput);
    // WB_LOGV(
    //     "ECTransition::evaluateTransitionWithEventInput(currentECState=%s, "
    //     "eventInput=%s, sourceECState=%s, ), result= %s",
    //     currentECState.getName().c_str(), eventInput.getName().c_str(),
    //     sourceECState.getName().c_str(), result ? "true" : "false");
    return result;
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
    if (ecState == nullptr) {
      WB_LOGE("new ECState(%s) is failed!", nameOfECState.c_str());
      return nullptr;
    }

    if (currentECState == nullptr) {
      currentECState = ecState;
    }

    ecStates.push_back(ecState);
    return ecState;
  }

  void addECAction(const String& nameOfECState, const String& nameOfAlgorithm,
                   const String& nameOfEventOutput) {
    ECState* ecState = findECStateByName(nameOfECState);
    Algorithm* algorithm = findAlgorithmByName(nameOfAlgorithm);
    EventOutput* eventOutput = findEventOutputByName(nameOfEventOutput);
    if (ecState && (algorithm || eventOutput)) {
      ecState->addECAction(algorithm, eventOutput);
    }
  }

  // static void addECAction(ECState& ecState, Algorithm* algorithm,
  //                         EventOutput* eventOutput) {
  //   ecState.addECAction(algorithm, eventOutput);
  // }

  // addECTransitionWithEvent()
  void addECTransition(const String& nameOfSourceECState,
                       const String& nameOfDestinationECState,
                       const String& nameOfECTransitionEvent,
                       const GuardCondition guardCondition = nullptr) {
    ECState* sourceECState = findECStateByName(nameOfSourceECState);
    if (sourceECState == nullptr) {
      WB_LOGE("sourceECState(%s) is NULL!", nameOfSourceECState.c_str());
      return;
    }

    ECState* destinationECState = findECStateByName(nameOfDestinationECState);
    if (destinationECState == nullptr) {
      WB_LOGE("ecTransitionEvent(%s) is required!",
              nameOfDestinationECState.c_str());
      return;
    }

    const ECTransitionEvent* ecTransitionEvent =
        findEventInputByName(nameOfECTransitionEvent);
    if (ecTransitionEvent == nullptr) {
      WB_LOGE("ecTransitionEvent(%s) is required!",
              nameOfECTransitionEvent.c_str());
      return;
    }

    _addECTransition(*sourceECState, *destinationECState, ecTransitionEvent,
                     guardCondition, false);
  }
  // addECTransitionWithBoolExpression()
  void addECTransition(const String& nameOfSourceECState,
                       const String& nameOfDestinationECState,
                       const GuardCondition guardCondition) {
    ECState* sourceECState = findECStateByName(nameOfSourceECState);
    ECState* destinationECState = findECStateByName(nameOfDestinationECState);

    if (sourceECState == nullptr || destinationECState == nullptr ||
        guardCondition == nullptr) {
      // TODO: print(ERROR, "guardCondition is required!");
      return;
    }
    _addECTransition(*sourceECState, *destinationECState, nullptr,
                     guardCondition, false);
  }
  // addECTransitionWithUnconditional()
  void addECTransition(const String& nameOfSourceECState,
                       const String& nameOfDestinationECState) {
    ECState* sourceECState = findECStateByName(nameOfSourceECState);
    ECState* destinationECState = findECStateByName(nameOfDestinationECState);

    if (sourceECState == nullptr || destinationECState == nullptr) {
      // TODO: print(ERROR, "guardCondition is required!");
      return;
    }
    _addECTransition(*sourceECState, *destinationECState, nullptr, nullptr,
                     true);
  }

  void executeEventInput(const EventInput& eventInput) {
    WB_OUT(
        "%s \tProcess: \tEVENT_INPUT \t%s \t\tWITH \t... \t(* ..., "
        "\t\tline:%d *) \r\n",
        getName().c_str(), eventInput.getName().c_str(),
        // (*state) ? "true" : "false",
        __LINE__);

    // WB_LOGD(
    //     "ECCBasicFBType::executeEventInput(FBType:%s, "
    //     "currentECState=%s, eventInput=%s)",
    //     getName().c_str(), currentECState->getName().c_str(),
    //     eventInput.getName().c_str());

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

 protected:
  template <typename T>
  /*const*/ T& vi(const String& nameOfInputVariable) {
    Vi<T>* inputVariable = (Vi<T>*)findInputVariableByName(nameOfInputVariable);
    if (inputVariable) {
      return inputVariable->getDataBox();
    } else {
      throw InputVariableException();
    }
  }

  template <typename T>
  T& vo(const String& nameOfOutputVariable) {
    Vo<T>* outputVariable =
        (Vo<T>*)findOutputVariableByName(nameOfOutputVariable);
    if (outputVariable) {
      return outputVariable->getDataBox();
    } else {
      throw OutputVariableException();
    }
  }

  template <typename T>
  T& vt(const String& nameOfInternalVariable) {
    Vt<T>* internalVariable =
        (Vt<T>*)findInternalVariableByName(nameOfInternalVariable);
    if (internalVariable) {
      return internalVariable->getDataBox();
    } else {
      throw InputVariableException();
    }
  }

 private:
  // ECCBasicFBType(const char *) {}

  void _addECTransition(ECState& sourceECState, ECState& destinationECState,
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

  ECTransition* evaluateTransitionWithEventInput(const EventInput& eventInput) {
    // WB_LOGD(
    //     "ECCBasicFBType::evaluateTransitionWithEventInput(eventInput=%s, "
    //     "ecTransitions.size()=%u)",
    //     eventInput.getName().c_str(), ecTransitions.size());

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

  void generateEventOutput(EventOutput& outEvent) {
    FBType::generateEventOutput(outEvent);
  }

  // ECC： Execuion Control Chart
  ECState* currentECState;
  std::list<ECState*> ecStates;            // length greater than 1
  std::list<ECTransition*> ecTransitions;  // length greater than 1

  std::list<Algorithm*> algorithms;  // 0..*

  friend class ECAction;  // ECCBasicFBType::generateEventOutput()
};

WOODBLOCK_END_PUBLIC_NAMESPACE
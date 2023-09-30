// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#include <WoodBlock/Macro.h>
#include <WoodBlock/Namespace.hpp>

#include <WoodBlock/Core/ECCBasicFBType.h>
#include <WoodBlock/Core/FBType.h>
#include <WoodBlock/Core/NamedObject.h>

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

ECState::~ECState() {
  for (std::list<ECAction*>::iterator it = ecActions.begin();
       it != ecActions.end(); it++) {
    delete *it;
  }
  ecActions.clear();
}

void ECState::addECAction(Algorithm* algorithm, EventOutput* eventOutput) {
  if (algorithm == nullptr && eventOutput == nullptr) {
    // TODO: print("Error: There must be one of the two!")
    return;
  }

  ECAction* ecAction = new ECAction(eccBasicFBType, algorithm, eventOutput);
  if (ecAction) {
    ecActions.push_back(ecAction);
  }
}

void ECState::executeECActions() {
  for (std::list<ECAction*>::iterator it = ecActions.begin();
       it != ecActions.end(); it++) {
    (*it)->execute();
  }
}

void ECAction::execute() {
  if (algorithm) {
    algorithm->execute();
  }
  if (eventOutput) {
    eccBasicFBType.generateEventOutput(*eventOutput);  //
  }
}

template <typename T>
const T& Algorithm::vi(const String& nameOfInputVariable) {
  Vi<T>* inputVariable =
      (Vi<T>*)eccBasicFBType.findInputVariableByName(nameOfInputVariable);
  if (inputVariable) {
    return inputVariable->getDataBox();
  } else {
    throw InputVariableException();
  }
}

template <typename T>
T& Algorithm::vo(const String& nameOfOutputVariable) {
  Vo<T>* outputVariable =
      (Vo<T>*)eccBasicFBType.findOutputVariableByName(nameOfOutputVariable);
  if (outputVariable) {
    return outputVariable->getDataBox();
  } else {
    throw OutputVariableException();
  }
}

template <typename T>
T& Algorithm::vt(const String& nameOfInternalVariable) {
  Vt<T>* internalVariable =
      (Vt<T>*)eccBasicFBType.findInternalVariableByName(nameOfInternalVariable);
  if (internalVariable) {
    return internalVariable->getDataBox();
  } else {
    throw InputVariableException();
  }
}

WOODBLOCK_END_PUBLIC_NAMESPACE
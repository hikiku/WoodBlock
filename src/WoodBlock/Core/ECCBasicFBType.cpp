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

WOODBLOCK_END_PUBLIC_NAMESPACE
// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#include <WoodBlock/Core/Event.h>
#include <WoodBlock/Core/FBInstance.h>

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

bool EventInput::addInVariableByName(const char* inVariableName) {
  bool result = check4AddingInVariableByName(inVariableName);
  if (!result) {
    return false;
  } else {
    InputVariable* inData = getOwner().findInVariableByName(inVariableName);
    inputVariables.push_back(inData);
    return true;
  }
}

bool EventInput::check4AddingInVariableByName(const char* inVariableName) {
  if (inVariableName == nullptr) {
    // TODO: printf (DEBUG, "inVariableName is NULL!");
    return false;
  }

  if (findInVariableByName(inVariableName)) {
    // TODO: printf (WARNING, "inVariableName(%s) is already in the inputVariables
    // list!", inVariableNames);
    return false;
  }

  InputVariable* inData = getOwner().findInVariableByName(inVariableName);
  if (!inData) {
    // TODO: printf (WARNING, "Don't find InVariable by  inVariableName(%s)!",
    // inVariableName);
    return false;
  } else {
    return true;
  }
}

void EventInput::disconnect() {
  if (this->outEvent == nullptr) {
    return;
  }
  // disconnect all of inputVariables
  for (std::list<InputVariable*>::iterator it = inputVariables.begin();
       it != inputVariables.end(); ++it) {
    (*it)->disconnect();
  }

  this->outEvent->clearConnectDestination();
  this->outEvent = nullptr;
}

void EventInput::trigger() {
  getOwner().processInEvent(*this);
}

bool EventOutput::addOutVariableByName(const char* outVariableName) {
  bool result = check4AddingOutVariableByName(outVariableName);
  if (!result) {
    return false;
  } else {
    OutputVariable* outData = getOwner().findOutVariableByName(outVariableName);
    outputVariables.push_back(outData);
    return true;
  }
}

bool EventOutput::check4AddingOutVariableByName(const char* outVariableName) {
  if (outVariableName == nullptr) {
    // TODO: printf (DEBUG, "outVariableName is NULL!");
    return false;
  }

  if (findOutVariableByName(outVariableName)) {
    // TODO: printf (WARNING, "outVariableName is already in the outputVariables
    // list!");
    return false;
  }

  OutputVariable* outData = getOwner().findOutVariableByName(outVariableName);
  if (!outData) {
    // TODO: printf (WARNING, "Don't find OutVariable by  outVariableName(%s)!",
    // outVariableName);
    return false;
  } else {
    return true;
  }
}

WOODBLOCK_END_PUBLIC_NAMESPACE
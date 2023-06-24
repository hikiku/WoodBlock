// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#include <WoodBlock/Core/Variable.h>

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

// check that outVariable and inVariable are match
bool VariableInput::checkForConnectFrom(VariableOutput& outData) const {
  if (isAlreadyConnected()) {
    // TODO: printf(WARN, "inData is alreay connected!\n");
    return false;
  }
  // check data type
  unsigned int outNumberOfDataType = outData.getNumberOfDataType();
  unsigned int inNumberOfDataType = getNumberOfDataType();
  bool result = check4ConnectDataType(outNumberOfDataType, inNumberOfDataType);
  if (!result) {
    // TODO: printf(WARNING, "outNumberOfDataType(%u) and inNumberOfDataType(%u) are
    // not matched!\n", outNumberOfDataType, inNumberOfDataType)
    return false;
  } else {
    return true;
  }
}

WOODBLOCK_END_PUBLIC_NAMESPACE

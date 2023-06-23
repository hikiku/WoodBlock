// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#include "Variable.h"

// check that outVariable and inVariable are match
bool VariableInput::checkForConnectFrom(VariableOutput &outData) const 
{
    if (isAlreadyConnected())
    {
        // TODO: printf(WARN, "inData is alreay connected!\n");
        return false;
    }
    // check data type
    unsigned int outDataType = outData.getDataType();
    unsigned int inDataType = getDataType();
    bool result = check4ConnectDataType(outDataType, inDataType);
    if (!result)
    {
        // TODO: printf(WARNING, "DataType of outDataType(%u) and inDataType(%u) do not match!\n", outDataTyp, inDataType)
        return false;
    }
    else
    {
        return true;
    }
}
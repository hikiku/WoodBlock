#include "WoodEvent.h"
#include "WoodBlock.h"

bool WoodInEvent::addInVariableByName(const char *inVariableName)
{
    bool result = check4AddingInVariableByName(inVariableName);
    if (!result)
    {
        return false;
    }
    else
    {
        WoodInData *inData = getOwner().findInVariableByName(inVariableName);
        inVariables.push_back(inData);
        return true;
    }
}

bool WoodInEvent::check4AddingInVariableByName(const char *inVariableName)
{
    if (inVariableName == nullptr)
    {
        // TODO: printf (DEBUG, "inVariableName is NULL!");
        return false;
    }

    if (findInVariableByName(inVariableName))
    {
        // TODO: printf (WARNING, "inVariableName(%s) is already in the inVariables list!", inVariableNames);
        return false;
    }

    WoodInData *inData = getOwner().findInVariableByName(inVariableName);
    if (!inData)
    {
        // TODO: printf (WARNING, "Don't find InVariable by  inVariableName(%s)!", inVariableName);
        return false;
    }
    else
    {
        return true;
    }
}

void WoodInEvent::disconnect()
{
    if (this->outEvent == nullptr)
    {
        return;
    }
    // disconnect all of inVariables
    for (std::list<WoodInData *>::iterator it = inVariables.begin(); it != inVariables.end(); ++it)
    {
        (*it)->disconnect();
    }

    this->outEvent->clearConnectDestination();
    this->outEvent = nullptr;
}

void WoodInEvent::trigger()
{
    getOwner().processInEvent(*this);
}

bool WoodOutEvent::addOutVariableByName(const char *outVariableName)
{
    bool result = check4AddingOutVariableByName(outVariableName);
    if (!result)
    {
        return false;
    }
    else
    {
        WoodOutData *outData = getOwner().findOutVariableByName(outVariableName);
        outVariables.push_back(outData);
        return true;
    }
}

bool WoodOutEvent::check4AddingOutVariableByName(const char *outVariableName)
{
    if (outVariableName == nullptr)
    {
        // TODO: printf (DEBUG, "outVariableName is NULL!");
        return false;
    }

    if (findOutVariableByName(outVariableName))
    {
        // TODO: printf (WARNING, "outVariableName is already in the outVariables list!");
        return false;
    }

    WoodOutData *outData = getOwner().findOutVariableByName(outVariableName);
    if (!outData)
    {
        // TODO: printf (WARNING, "Don't find OutVariable by  outVariableName(%s)!", outVariableName);
        return false;
    }
    else
    {
        return true;
    }
}
#ifndef WoodBlock_class_h
#define WoodBlock_class_h
#ifdef __cplusplus

#include <list>
#include <iterator> //

#include <WString.h>

#include "WBMacro.h"
#include "WoodData.h"
// #include "WoodBlock.h"

class WoodBlock;
class WoodInEvent;
class WoodOutEvent;
// class WoodInData;
// class WoodOutData;

// Wood Event Type
#define EVENT_ANY (0)
#define EVENT_USER_EXTENDED_BASE (10000)

class WoodEvent
{
public:
    WoodEvent(WoodBlock &owner, const String &name, unsigned int eventType = EVENT_ANY)
        : owner(owner), name(name), eventType(eventType) {}
    virtual ~WoodEvent() = 0 {}
    const String &getName() { return name; }
    unsigned int getEventType() { return eventType; }

protected:
    WoodBlock &getOwner() { return owner; }

private:
    WoodBlock &owner; // belongs to
    String name;
    unsigned int eventType;
};

class WoodInEvent : public WoodEvent
{
public:
    WoodInEvent(WoodBlock &owner, const char *name)
        : WoodEvent(owner, name), inVariables(), outEvent(nullptr)
    {
        //
    }
    ~WoodInEvent() {}

    WoodInData *findInVariableByName(const char *inVariableName)
    {
        if (inVariableName == nullptr)
        {
            // printf (DEBUG, "inVariableName is NULL!");
            return nullptr;
        }

        String name(inVariableName);
        for (std::list<WoodInData &>::iterator it = inVariables.begin(); it != inVariables.end(); ++it)
        {
            if (it->getName().equals(name))
            {
                return &(*it);
            }
        }

        return nullptr;
    }

    bool addInVariableByName(const char *inVariableName)
    {
        bool result = check4AddingInVariableByName(inVariableName);
        if (!result)
        {
            return false;
        }
        else
        {
            WoodInData *inData = getOwner().findInVariableByName(inVariableName);
            inVariables.push_back(*inData);
            return true;
        }
    }

    bool addInVariablesByNames(const char *inVariableNames[], int sizeofInVariables)
    {
        bool result = check4AddingInVariablesByNames(inVariableNames, sizeofInVariables);
        if (!result)
        {
            return false;
        }

        // add
        for (int i = 0; i < sizeofInVariables; i++)
        {
            bool result = addInVariableByName(inVariableNames[i]);
            if (!result)
            {
                // TODO: printf (ERROR, "inVariableNames[%d] is error!", %d);
                return false;
            }
        }
        return true;
    }

    bool isAlreadyConnected() { return (outEvent == nullptr) ? false : true; }
    bool setConnectSource(WoodOutEvent &outEvent) // friend class method
    {
        if (this->outEvent == nullptr)
        {
            this->outEvent = &outEvent;
            return true;
        }
        return false;
    }
    void disconnect()
    {
        if (this->outEvent == nullptr)
        {
            return;
        }
        // disconnect all of inVariables
        for (std::list<WoodInData &>::iterator it = inVariables.begin(); it != inVariables.end(); ++it)
        {
            it->disconnect();
        }

        this->outEvent->clearConnectDestination();
        this->outEvent == nullptr;
    }

    bool trigger() // TODO: move outside????
    {
        getOwner().processInEvent(*this);
    }

    void sample()
    {
        // sample all of inVariables
        for (std::list<WoodInData &>::iterator it = inVariables.begin(); it != inVariables.end(); ++it)
        {
            it->sample();
        }
    }

private:
    bool check4AddingInVariableByName(const char *inVariableName)
    {
        if (inVariableName == nullptr)
        {
            // TODO: printf (DEBUG, "inVariableName is NULL!");
            return false;
        }

        if (findInVariableByName(inVariableName))
        {
            // TODO: printf (WARNING, "inVariableName is already in the inVariables list!");
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

    bool check4AddingInVariablesByNames(const char *inVariables[], int sizeofInVariables)
    {
        if (!inVariables)
        {
            // TODO: printf (ERROR, "inVariables is NULL!");
            return false;
        }
        if (sizeofInVariables <= 0)
        {
            // TODO: printf (ERROR, "sizeofInVariables(%d) is <= 0!", sizeofInVariables);
            return false;
        }

        // check
        for (int i = 0; i < sizeofInVariables; i++)
        {
            bool result = check4AddingInVariableByName(inVariables[i]);
            if (!result)
            {
                // TODO: printf (ERROR, "inVariables[%d] is error!", %d);
                return false;
            }
        }
        return true;
    }

    std::list<WoodInData &> inVariables;
    WoodOutEvent *outEvent;
};

class WoodOutEvent : public WoodEvent
{
public:
    WoodOutEvent(WoodBlock &owner, const char *name)
        : WoodEvent(owner, name), outVariables(), inEvent(nullptr) {}
    ~WoodOutEvent()
    {
        // std::list<WoodOutData &> outVariables;
        outVariables.clear();
    }

    bool addOutVariableByName(const char *outVariableName)
    {
        bool result = check4AddingOutVariableByName(outVariableName);
        if (!result)
        {
            return false;
        }
        else
        {
            WoodOutData *outData = getOwner().findOutVariableByName(outVariableName);
            outVariables.push_back(*outData);
            return true;
        }
    }

    bool addOutVariablesByNames(const char *outVariableNames[], int sizeofOutVariables)
    {
        bool result = check4AddingOutVariablesByNames(outVariableNames, sizeofOutVariables);
        if (!result)
        {
            return false;
        }

        // add
        for (int i = 0; i < sizeofOutVariables; i++)
        {
            bool result = addOutVariableByName(outVariableNames[i]);
            if (!result)
            {
                // TODO: printf (ERROR, "outVariables[%d] is error!", %d);
                return false;
            }
        }
        return true;
    }

    WoodOutData *findOutVariableByName(const char *outVariableName)
    {
        if (outVariableName == nullptr)
        {
            // printf (DEBUG, "outVariableName is NULL!");
            return nullptr;
        }

        String name(outVariableName);
        for (std::list<WoodOutData &>::iterator it = outVariables.begin(); it != outVariables.end(); ++it)
        {
            if (it->getName().equals(name))
            {
                return &(*it);
            }
        }

        return nullptr;
    }

    bool isAlreadyConnected() { return (inEvent == nullptr) ? false : true; }
    bool connectTo(const char *outVariableNames[],
                   WoodInEvent &inEvent, const char *inVariableNames[], int sizeofVariables)
    {
        bool result = true;

        // Whether the Event Type of A and B match
        unsigned int srcEventType = getEventType();
        unsigned int dstEventType = inEvent.getEventType();
        if ((dstEventType != EVENT_ANY) && (dstEventType != srcEventType))
        {
            // TODO: printf(WARNING, "EventType of srcEventType(%u) and dstEventType(%u) do not match!\n", srcEventType, dstEventType)
            result = false;
        }
        if (isAlreadyConnected())
        {
            // TODO: printf(WARNING, "outEvent is already connected!\n")
            result = false;
        }
        if (inEvent.isAlreadyConnected())
        {
            // TODO: printf(WARNING, "inEvent is already connected!\n")
            result = false;
        }

        // check that each of outVariableNames and each of inVariableNames are match
        for (int i = 0; i < sizeofVariables; i++)
        {
            WoodOutData *outVariable = findOutVariableByName(outVariableNames[i]);
            WoodInData *inVariable = inEvent.findInVariableByName(inVariableNames[i]);

            if (!outVariable)
            {
                //// printf(ERROR, "Check: It Can't find WoodOutData by name %s!\n", outVariableNames[i])
                result = false;
            }
            if (!inVariable)
            {
                //// printf(ERROR, "Check: It Can't find WoodInData by name %s!\n", inVariableNames[i])
                result = false;
            }
            // check that outVariable and inVariable are match
            inVariable->checkForConnectFrom(*outVariable);
        }
        if (!result)
        {
            //// printf(HINT, "arguments of connection is error!\n");
            return false;
        }

        // connect inVariables from outVariables
        for (int i = 0; i < sizeofVariables; i++)
        {
            WoodOutData *outVariable = findOutVariableByName(outVariableNames[i]);
            WoodInData *inVariable = inEvent.findInVariableByName(inVariableNames[i]);

            if (!outVariable)
            {
                //// printf(ERROR, "Connect: It Can't find WoodOutData by name %s!\n", outVariableNames[i])
                continue; // result = false;
            }
            if (!inVariable)
            {
                //// printf(ERROR, "Connect: It Can't find WoodInData by name %s!\n", inVariableNames[i])
                continue; // result = false;
            }
            inVariable->connectFrom(*outVariable);
        }

        // connect outEvent to inEvent
        this->inEvent = &inEvent;
        inEvent.setConnectSource(*this);
        return true;
    }

    void disconnect()
    {
        if (inEvent)
        {
            inEvent->disconnect();
        }
    }
    void clearConnectDestination() // friend class method
    {
        if (inEvent)
        {
            inEvent = nullptr;
        }
    }

    void generate() { generated = true; }
    bool isGenerated() { return generated; }

    bool dispatchAndExecute()
    {
        // TODO:
        if (!isGenerated())
        {
            return false;
        }
        if (!inEvent)
        {
            return false;
        }
        inEvent->trigger();
        clear();
    }

private:
    bool check4AddingOutVariableByName(const char *outVariableName)
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

    bool check4AddingOutVariablesByNames(const char *outVariables[], int sizeofOutVariables)
    {
        if (!outVariables)
        {
            // TODO: printf (ERROR, "outVariables is NULL!");
            return false;
        }
        if (sizeofOutVariables <= 0)
        {
            // TODO: printf (ERROR, "sizeofOutVariables(%d) is <= 0!", sizeofOutVariables);
            return false;
        }

        // check
        for (int i = 0; i < sizeofOutVariables; i++)
        {
            bool result = check4AddingOutVariableByName(outVariables[i]);
            if (!result)
            {
                // TODO: printf (ERROR, "outVariables[%d] is error!", %d);
                return false;
            }
        }
        return true;
    }

    void clear() { generated = false; }

    std::list<WoodOutData &> outVariables;
    WoodInEvent *inEvent; // event observer, to event, connect to
    bool generated;       // Has a out event been alreay generated?
};

class WoodServiceInterfaceInEvent : public WoodInEvent
{
    //
};

#endif // __cplusplus
#endif // WoodEvent
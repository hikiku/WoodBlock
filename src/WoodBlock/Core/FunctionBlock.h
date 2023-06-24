// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <list>
#include <iterator> //
#include <WString.h> // Arduino

#include <WoodBlock/Namespace.hpp>
#include <WoodBlock/Core/Macro.h>
#include <WoodBlock/Core/Variable.h>
#include <WoodBlock/Core/Event.h>

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

class FunctionBlock
{
public:
    FunctionBlock(const char *name)
        : name(name), inEvents(), outEvents(), inVariables(), outVariables(), triggeredOutEvents() {}
    virtual ~FunctionBlock() // TODO: = 0;
    {
        // std::list<EventOutput*> triggeredOutEvents;
        triggeredOutEvents.clear();

        // clear some connects of inEvents & outEvents, inVariables & outVariables
        disconnect();

        // remove member variables
        // std::list<EventInput*> inEvents;
        for (std::list<EventInput *>::iterator it = inEvents.begin(); it != inEvents.end(); ++it)
        {
            delete *it;
        }
        inEvents.clear();
        // std::list<EventOutput*> outEvents;
        for (std::list<EventOutput *>::iterator it = outEvents.begin(); it != outEvents.end(); ++it)
        {
            delete *it;
        }
        outEvents.clear();
        // std::list<VariableInput*> inVariables;
        for (std::list<VariableInput *>::iterator it = inVariables.begin(); it != inVariables.end(); ++it)
        {
            delete *it;
        }
        inVariables.clear();
        // std::list<VariableOutput*> outVariables;
        for (std::list<VariableOutput *>::iterator it = outVariables.begin(); it != outVariables.end(); ++it)
        {
            delete *it;
        }
        outVariables.clear();
    }

    const String &getName()
    {
        return name;
    }

    EventInput *findInEventByName(const String &inEventName)
    {
        // std::list<EventInput*> inEvents;
        for (std::list<EventInput *>::iterator it = inEvents.begin(); it != inEvents.end(); ++it)
        {
            if ((*it)->getName().equals(inEventName))
            {
                return *it;
            }
        }

        return nullptr;
    }
    EventOutput *findOutEventByName(const String &outEventName)
    {
        // std::list<EventOutput*> outEvents;
        for (std::list<EventOutput *>::iterator it = outEvents.begin(); it != outEvents.end(); ++it)
        {
            if ((*it)->getName().equals(outEventName))
            {
                return *it;
            }
        }

        return nullptr;
    }
    VariableInput *findInVariableByName(const String &inVariableName)
    {
        // std::list<VariableInput*> inVariables;
        for (std::list<VariableInput *>::iterator it = inVariables.begin(); it != inVariables.end(); ++it)
        {
            if ((*it)->getName().equals(inVariableName))
            {
                return *it;
            }
        }

        return nullptr;
    }
    VariableOutput *findOutVariableByName(const String &outVariableName)
    {
        // std::list<VariableOutput*> outVariables;
        for (std::list<VariableOutput *>::iterator it = outVariables.begin(); it != outVariables.end(); ++it)
        {
            if ((*it)->getName().equals(outVariableName))
            {
                return *it;
            }
        }

        return nullptr;
    }

    // ====================== Constructor: adding =========================
    // eg: DataBoxSInt
    template <class TDataBox>
    VariableInputImpl<TDataBox> *addInVariable(const char *name)
    {
        // push a inVariable to std::list<VariableInput*> inVariables!
        VariableInputImpl<TDataBox> *inVariable = new VariableInputImpl<TDataBox>(name);
        if (inVariable)
        {
            inVariables.push_back(inVariable);
            return inVariable;
        }
        return nullptr;
    }
    // eg: DataBoxSInt
    template <class TDataBox>
    VariableOutputImpl<TDataBox> *addOutVariable(const char *name)
    {
        // push a outVariable to std::list<VariableOutput*> outVariables!
        VariableOutputImpl<TDataBox> *outVariable = new VariableOutputImpl<TDataBox>(name); // VariableOutput
        if (outVariable)
        {
            outVariables.push_back(outVariable);
            return outVariable;
        }
        return nullptr;
    }

    // eg: EVENT_ANY, ...
    // template <class TEventType>
    EventInput *addInEvent(const char *name, const char *inVariableNames[], int sizeofInVariables)
    {
        EventInput *inEvent = new EventInput(*this, name);
        if (inEvent)
        {
            bool result = inEvent->addInVariablesByNames(inVariableNames, sizeofInVariables);
            // add inEvents ot std::list<EventInput*> inEvents;
            if (result)
            {
                inEvents.push_back(inEvent);
                return inEvent;
            }
            else
            {
                // TODO: printf (ERROR, "It fail for calling addInVariablesByNames(EventInput[%s])!", name);
                delete inEvent;
                return nullptr;
            }
        }
        else
        {
            // TODO: printf (ERROR, "It fail for adding EventInput[%s]!", name);
            return nullptr;
        }
    }
    // eg: EVENT_ANY, ...
    // template <class TEventType>
    EventOutput *addOutEvent(const char *name, const char *outVariableNames[], int sizeofOutVariables)
    {
        // TODO: ...
        EventOutput *outEvent = new EventOutput(*this, name);
        if (outEvent)
        {
            bool result = outEvent->addOutVariablesByNames(outVariableNames, sizeofOutVariables);
            // add outEvent to std::list<EventOutput*> outEvents;
            if (result)
            {
                outEvents.push_back(outEvent);
                return outEvent;
            }
            else
            {
                // TODO: printf (ERROR, "It fail for calling addOutVariablesByNames(EventInput[%s])!", name);
                delete outEvent;
                return nullptr;
            }
        }
        else
        {
            // TODO: printf (ERROR, "It fail for adding addOutEvent[%s]!", name);
            return nullptr;
        }
    }

    // ====================== Constructor: connecting =========================
    bool connectTo(const char *outEventName, const char *outVariableNames[],
                   FunctionBlock &destFunctionBlock, const char *inEventName, const char *inVariableNames[], int sizeofVariables)
    {
        bool result = true;
        EventOutput *outEvent = findOutEventByName(outEventName);
        EventInput *inEvent = destFunctionBlock.findInEventByName(inEventName);
        if (!outEvent)
        {
            //// printf(ERROR, "It Can't find EventOutput by name %s!\n", outEventName)
            result = false;
        }
        if (!inEvent)
        {
            //// printf(ERROR, "It Can't find EventInput by name %s!\n", inEventName)
            result = false;
        }
        if (!result)
        {
            return false;
        }

        return outEvent->connectTo(outVariableNames, *inEvent, inVariableNames, sizeofVariables);
    }

    // ====================== Normal: running =========================
    void processInEvent(EventInput &inEvent)
    {
        inEvent.sample();                 // sample input variables
        executeInEvent(inEvent);          // execution ecc
        dispatchAndExecuteAllOutEvents(); // dispatch and execute all output events
    }

    // ====================== Deconstructor: disconnecting =========================
    void disconnect()
    {

        // std::list<EventOutput*> triggeredOutEvents;
        triggeredOutEvents.clear();

        // std::list<EventInput*> inEvents;
        for (std::list<EventInput *>::iterator it = inEvents.begin(); it != inEvents.end(); ++it)
        {
            (*it)->disconnect();
        }

        // std::list<EventOutput*> outEvents;
        for (std::list<EventOutput *>::iterator it = outEvents.begin(); it != outEvents.end(); ++it)
        {
            (*it)->disconnect();
        }
    }

    // ====================== Deconstructor: removing =========================
    // Some private functions in ~FunctionBlock()

protected:
    virtual void executeInEvent(EventInput &inEvent) = 0;
    //{
    // TODO: ......
    // if inEvent.is("INIT") { ... }
    // else if inEvent.is("RESET") { ... }
    // ...... OutData.set(xyz)......
    // ...... generateOutEvent(EventOutput &outEvent)......
    //}

    void generateOutEvent(EventOutput &outEvent)
    {
        outEvent.generate();
        triggeredOutEvents.push_back(&outEvent);
    }

    // dispatch and execute all output events
    void dispatchAndExecuteAllOutEvents()
    {
        for (std::list<EventOutput *>::iterator it = triggeredOutEvents.begin(); it != triggeredOutEvents.end();)
        {
            (*it)->dispatchAndExecute();
            it = triggeredOutEvents.erase(it); // it++;
        }
        // triggeredOutEvents.clear();
    }

private:
    void disconnectInEvent(const String &inEventName)
    {
        EventInput *inEvent = findInEventByName(inEventName);
        if (inEvent)
        {
            inEvent->disconnect();
        }
    }
    void disconnectOutEvent(const String &outEventName)
    {
        EventOutput *outEvent = findOutEventByName(outEventName);
        if (outEvent)
        {
            outEvent->disconnect();
        }
    }

    String name;

    std::list<EventInput *> inEvents;
    std::list<EventOutput *> outEvents;

    std::list<VariableInput *> inVariables;
    std::list<VariableOutput *> outVariables;

    std::list<EventOutput *> triggeredOutEvents; // pop_front() & push_back(), erase()/clear()
};

class ServiceInterfaceBlock : public FunctionBlock
{
public:
    ServiceInterfaceBlock(const char *name) : FunctionBlock(name) {} //, siiEvents()
    ~ServiceInterfaceBlock()
    {
        // // std::list<ServiceInterfaceInEvent*> siiEvents;
        // for (std::list<ServiceInterfaceInEvent*>::iterator it = siiEvents.begin(); it != siiEvents.end();)
        // {
        //     delete *it;
        //     it = siiEvents.erase(it); // it++;
        // }
    }

    bool fetchExternalEvents()
    {
        bool result = captureAndExecuteServiceInterfaceInEvent(); // execution ecc
        if (result)
        {
            dispatchAndExecuteAllOutEvents(); // dispatch and execute all output events
        }
        return true;
    }

protected:
    virtual bool captureAndExecuteServiceInterfaceInEvent() = 0;
    // {
    //     // Get extra/sifb event and/or timer event
    //     ServiceInterfaceInEvent *siiEvent = captureServiceInterfaceInEvent();
    //     // if no extra/sifb event, return false;
    //     if ((siiEvent == nullptr) || (siiEvent == NULL))
    //     {
    //         //// printf (HINT, "It has no ServiceInterfaceInEvent to capture!\n"])
    //         return false;
    //     }
    //     // virtual ServiceInterfaceInEvent *captureServiceInterfaceInEvent() = 0;
    //     executionServiceInterfaceInEvent(*siiEvent);
    //     // virtual void executionServiceInterfaceInEvent(ServiceInterfaceInEvent &siiEvent) = 0;
    //     // {
    //     //   TODO: ......
    //     //   if siiEvent.is("AAA") { ... }
    //     //   else if siiEvent.is("BBB") { ... }
    //     //   ...... OutData.set(xyz)......
    //     //   ...... generateOutEvent(EventOutput &outEvent)......
    //     //  }
    //     return true;
    // }

private:
    // std::list<ServiceInterfaceInEvent*> siiEvents;
};

WOODBLOCK_END_PUBLIC_NAMESPACE
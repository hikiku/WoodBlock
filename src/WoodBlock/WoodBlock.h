#ifndef WoodBlock_class_h
#define WoodBlock_class_h
#ifdef __cplusplus

#include <list>
#include <iterator> //

#include <WString.h>

#include "WoodMacro.h"
#include "WoodData.h"
#include "WoodEvent.h"

class WoodBlock
{
public:
    WoodBlock(const String &name)
        : name(name), inEvents(), outEvents(), inVariables(), outVariables(), triggeredOutEvents() {}
    virtual ~WoodBlock() // TODO: = 0;
    {
        // std::list<WoodOutEvent*> triggeredOutEvents;
        triggeredOutEvents.clear();

        // clear some connects of inEvents & outEvents, inVariables & outVariables
        disconnect();

        // remove member variables
        // std::list<WoodInEvent*> inEvents;
        for (std::list<WoodInEvent *>::iterator it = inEvents.begin(); it != inEvents.end(); ++it)
        {
            delete *it;
        }
        inEvents.clear();
        // std::list<WoodOutEvent*> outEvents;
        for (std::list<WoodOutEvent *>::iterator it = outEvents.begin(); it != outEvents.end(); ++it)
        {
            delete *it;
        }
        outEvents.clear();
        // std::list<WoodInData*> inVariables;
        for (std::list<WoodInData *>::iterator it = inVariables.begin(); it != inVariables.end(); ++it)
        {
            delete *it;
        }
        inVariables.clear();
        // std::list<WoodOutData*> outVariables;
        for (std::list<WoodOutData *>::iterator it = outVariables.begin(); it != outVariables.end(); ++it)
        {
            delete *it;
        }
        outVariables.clear();
    }

    const String &getName()
    {
        return name;
    }

    WoodInEvent *findInEventByName(const String &inEventName)
    {
        // std::list<WoodInEvent*> inEvents;
        for (std::list<WoodInEvent *>::iterator it = inEvents.begin(); it != inEvents.end(); ++it)
        {
            if ((*it)->getName().equals(inEventName))
            {
                return *it;
            }
        }

        return nullptr;
    }
    WoodOutEvent *findOutEventByName(const String &outEventName)
    {
        // std::list<WoodOutEvent*> outEvents;
        for (std::list<WoodOutEvent *>::iterator it = outEvents.begin(); it != outEvents.end(); ++it)
        {
            if ((*it)->getName().equals(outEventName))
            {
                return *it;
            }
        }

        return nullptr;
    }
    WoodInData *findInVariableByName(const String &inVariableName)
    {
        // std::list<WoodInData*> inVariables;
        for (std::list<WoodInData *>::iterator it = inVariables.begin(); it != inVariables.end(); ++it)
        {
            if ((*it)->getName().equals(inVariableName))
            {
                return *it;
            }
        }

        return nullptr;
    }
    WoodOutData *findOutVariableByName(const String &outVariableName)
    {
        // std::list<WoodOutData*> outVariables;
        for (std::list<WoodOutData *>::iterator it = outVariables.begin(); it != outVariables.end(); ++it)
        {
            if ((*it)->getName().equals(outVariableName))
            {
                return *it;
            }
        }

        return nullptr;
    }

    // ====================== Constructor: adding =========================
    // eg: WoodSIntDataBox
    // WoodInDataImpl<WoodSIntDataBox>;
    template <class TDataBox>
    bool addInVariable(const char *name)
    {
        // push a inVariable to std::list<WoodInData*> inVariables!
        WoodInData *inVariable = new WoodInDataImpl<TDataBox>(name);
        if (inVariable)
        {
            inVariables.push_back(inVariable);
            return true;
        }
        return false;
    }
    // eg: WoodSIntDataBox
    template <class TDataBox>
    bool addOutVariable(const char *name)
    {
        // push a outVariable to std::list<WoodOutData*> outVariables!
        WoodOutData *outVariable = new WoodOutDataImpl<TDataBox>(name);
        if (outVariable)
        {
            outVariables.push_back(outVariable);
            return true;
        }
        return false;
    }

    // eg: EVENT_ANY, ...
    // template <class TEventType>
    bool addInEvent(const char *name, const char *inVariableNames[], int sizeofInVariables)
    {
        WoodInEvent *inEvent = new WoodInEvent(*this, name);
        if (inEvent)
        {
            bool result = inEvent->addInVariablesByNames(inVariableNames, sizeofInVariables);
            // add inEvents ot std::list<WoodInEvent*> inEvents;
            if (result)
            {
                inEvents.push_back(inEvent);
                return true;
            }
            else
            {
                // TODO: printf (ERROR, "It fail for calling addInVariablesByNames(WoodInEvent[%s])!", name);
                delete inEvent;
                return false;
            }
        }
        else
        {
            // TODO: printf (ERROR, "It fail for adding WoodInEvent[%s]!", name);
            return false;
        }
    }
    // eg: EVENT_ANY, ...
    // template <class TEventType>
    bool addOutEvent(const char *name, const char *outVariableNames[], int sizeofOutVariables)
    {
        // TODO: ...
        WoodOutEvent *outEvent = new WoodOutEvent(*this, name);
        if (outEvent)
        {
            bool result = outEvent->addOutVariablesByNames(outVariableNames, sizeofOutVariables);
            // add outEvent to std::list<WoodOutEvent*> outEvents;
            if (result)
            {
                outEvents.push_back(outEvent);
                return true;
            }
            else
            {
                // TODO: printf (ERROR, "It fail for calling addOutVariablesByNames(WoodInEvent[%s])!", name);
                delete outEvent;
                return false;
            }
        }
        else
        {
            // TODO: printf (ERROR, "It fail for adding addOutEvent[%s]!", name);
            return false;
        }
    }

    // ====================== Constructor: connecting =========================
    bool connectTo(const char *outEventName, const char *outVariableNames[],
                   WoodBlock &destWoodBlock, const char *inEventName, const char *inVariableNames[], int sizeofVariables)
    {
        bool result = true;
        WoodOutEvent *outEvent = findOutEventByName(outEventName);
        WoodInEvent *inEvent = destWoodBlock.findInEventByName(inEventName);
        if (!outEvent)
        {
            //// printf(ERROR, "It Can't find WoodOutEvent by name %s!\n", outEventName)
            result = false;
        }
        if (!inEvent)
        {
            //// printf(ERROR, "It Can't find WoodInEvent by name %s!\n", inEventName)
            result = false;
        }
        if (!result)
        {
            return false;
        }

        return outEvent->connectTo(outVariableNames, *inEvent, inVariableNames, sizeofVariables);
    }

    // ====================== Normal: running =========================
    void processInEvent(WoodInEvent &inEvent)
    {
        inEvent.sample();                 // sample input variables
        executionInEvent(inEvent);        // execution ecc
        dispatchAndExecuteAllOutEvents(); // dispatch and execute all output events
    }

    // ====================== Deconstructor: disconnecting =========================
    void disconnect()
    {

        // std::list<WoodOutEvent*> triggeredOutEvents;
        triggeredOutEvents.clear();

        // std::list<WoodInEvent*> inEvents;
        for (std::list<WoodInEvent *>::iterator it = inEvents.begin(); it != inEvents.end(); ++it)
        {
            (*it)->disconnect();
        }

        // std::list<WoodOutEvent*> outEvents;
        for (std::list<WoodOutEvent *>::iterator it = outEvents.begin(); it != outEvents.end(); ++it)
        {
            (*it)->disconnect();
        }
    }

    // ====================== Deconstructor: removing =========================
    // Some private functions in ~WoodBlock()

protected:
    virtual void executionInEvent(WoodInEvent &inEvent) = 0;
    //{
    // TODO: ......
    // if inEvent.is("INIT") { ... }
    // else if inEvent.is("RESET") { ... }
    // ...... woodOutData.set(xyz)......
    // ...... generateOutEvent(WoodOutEvent &outEvent)......
    //}

    void generateOutEvent(WoodOutEvent &outEvent)
    {
        outEvent.generate();
        triggeredOutEvents.push_back(&outEvent);
    }

    // dispatch and execute all output events
    void dispatchAndExecuteAllOutEvents()
    {
        for (std::list<WoodOutEvent *>::iterator it = triggeredOutEvents.begin(); it != triggeredOutEvents.end();)
        {
            (*it)->dispatchAndExecute();
            it = triggeredOutEvents.erase(it); // it++;
        }
        // triggeredOutEvents.clear();
    }

private:
    void disconnectInEvent(const String &inEventName)
    {
        WoodInEvent *inEvent = findInEventByName(inEventName);
        if (inEvent)
        {
            inEvent->disconnect();
        }
    }
    void disconnectOutEvent(const String &outEventName)
    {
        WoodOutEvent *outEvent = findOutEventByName(outEventName);
        if (outEvent)
        {
            outEvent->disconnect();
        }
    }

    String name;

    std::list<WoodInEvent *> inEvents;
    std::list<WoodOutEvent *> outEvents;

    std::list<WoodInData *> inVariables;
    std::list<WoodOutData *> outVariables;

    std::list<WoodOutEvent *> triggeredOutEvents; // pop_front() & push_back(), erase()/clear()
};

class WoodServiceInterfaceBlock : public WoodBlock
{
public:
    WoodServiceInterfaceBlock(const String &name) : WoodBlock(name) {} //, siiEvents()
    ~WoodServiceInterfaceBlock()
    {
        // // std::list<WoodServiceInterfaceInEvent*> siiEvents;
        // for (std::list<WoodServiceInterfaceInEvent*>::iterator it = siiEvents.begin(); it != siiEvents.end();)
        // {
        //     delete *it;
        //     it = siiEvents.erase(it); // it++;
        // }
    }

    bool fetchExternalEvents()
    {
        // Get extra/sifb event and/or timer event
        WoodServiceInterfaceInEvent *siiEvent = captureServiceInterfaceInEvent();
        // if no extra/sifb event, return false;
        if ((siiEvent == nullptr) || (siiEvent == NULL))
        {
            //// printf (HINT, "It has no ServiceInterfaceInEvent to capture!\n"])
            return false;
        }

        executionServiceInterfaceInEvent(*siiEvent); // execution ecc
        dispatchAndExecuteAllOutEvents();            // dispatch and execute all output events
        return true;
    }

protected:
    virtual WoodServiceInterfaceInEvent *captureServiceInterfaceInEvent() = 0;

    virtual void executionServiceInterfaceInEvent(WoodServiceInterfaceInEvent &siiEvent) = 0;
    //{
    // TODO: ......
    // if siiEvent.is("AAA") { ... }
    // else if siiEvent.is("BBB") { ... }
    // ...... woodOutData.set(xyz)......
    // ...... generateOutEvent(WoodOutEvent &outEvent)......
    //}

private:
    // std::list<WoodServiceInterfaceInEvent*> siiEvents;
};

#endif // __cplusplus
#endif // WoodBlock
// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once
#include <Arduino.h>  // Arduino
#include <WString.h>  // Arduino
#include <iterator>   //
#include <list>

#include <WoodBlock/Macro.h>
#include <WoodBlock/Namespace.hpp>

#include <WoodBlock/Core/Event.h>
#include <WoodBlock/Core/ManagedObject.h>
#include <WoodBlock/Core/Variable.h>

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

class ManagedConnection : public ManagedObject {
  // TODO:
};

class EventConnection : public ManagedConnection {
 public:
  EventConnection(FBInstance* sourceFBInstance, EventOutput* sourceEvent,
                  FBInstance* destFBInstance, EventInput* destEvent)
      : sourceFBInstance(sourceFBInstance),
        sourceEvent(sourceEvent),
        destFBInstance(destFBInstance),
        destEvent(destEvent) {}

  FBInstance* getSourceFBInstance() {
    return sourceFBInstance;
  }
  EventOutput* getSourceEvent() {
    return sourceEvent;
  }

  FBInstance* getDestinationFBInstance() {
    return destFBInstance;
  }
  EventInput* getDestinationEvent() {
    return destEvent;
  };

 private:
  FBInstance* sourceFBInstance;
  EventOutput* sourceEvent;  // from / source // outEvent

  FBInstance* destFBInstance;
  EventInput* destEvent;  // to / destination // inEvent
};

class DataConnection : public ManagedConnection {
 public:
  DataConnection(EventConnection* eventConnection, OutputVariable* source,
                 InputVariable* destination)
      : connection(eventConnection),
        outputVariable(source),
        inputVariable(destination) {}

  EventConnection* eventConnection() {
    return this->connection;
  };
  OutputVariable* source() {
    return this->outputVariable;
  };
  InputVariable* destination() {
    return this->inputVariable;
  };

 private:
  EventConnection* connection;
  OutputVariable* outputVariable;  // from / source // //outData
  InputVariable* inputVariable;    // to / destination
};

WOODBLOCK_END_PUBLIC_NAMESPACE
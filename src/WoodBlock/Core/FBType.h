// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once
#include <Arduino.h>  // Arduino
// #include <WString.h>  // Arduino
// #include <iterator>   //
// #include <list>

#include <WoodBlock/Macro.h>
#include <WoodBlock/Namespace.hpp>
#include <WoodBlock/Core/NamedObject.h>

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

class FBType :public NamedObject {
  // TODO: FBType：Plug, Socket, Service. create a FBInstance from CompiledCode & Declaraitons.

  // TODO: std::list<EventInput> eventInputs; // 0..*
  // TODO: std::list<EventOutput> eventOutputs; // 0..*

  // TODO: std::list<InputVariable> inputVariables; // 0..*
  // TODO: std::list<OutputVariable> outputVariables; // 0..*

  // TODO: std::list<Socket> sockets; // 0..*
  // TODO: std::list<Plug> plugs; // 0..*

  // TODO: std::list<Service> services; // 0..*
};

class BasicFBType :public FBType {
  // TODO:
  // TODO: Algorithm algorithm; // 0..1
  // TODO: ECC ecc; // 0..1
  // TODO: std::list<InternalVariable> internalVariables; // 0..*
};

class CompositeFBType :public FBType {
  // TODO:
  // TODO: FBNetwork: fbNetwork; // 1..1
};

class SIFBType :public FBType {
  // TODO:
};

WOODBLOCK_END_PUBLIC_NAMESPACE
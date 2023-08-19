// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once
#include <Arduino.h>  // Arduino
#include <WString.h>  // Arduino
#include <iterator>   //
#include <list>

#include <WoodBlock/Namespace.hpp>
#include <WoodBlock/Macro.h>

#include <WoodBlock/Core/Variable.h>
#include <WoodBlock/Core/Event.h>
#include <WoodBlock/Core/ManagedObject.h>

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

class ManagedConnection :public ManagedObject{
    // TODO:    
};

class EventConnection :public ManagedConnection {
    // TODO:
};

class DataConnection :public ManagedConnection {
    // TODO:  
};

WOODBLOCK_END_PUBLIC_NAMESPACE
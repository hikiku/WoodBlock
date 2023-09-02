// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once
#include <Arduino.h>  // Arduino
// #include <WString.h>  // Arduino
// #include <iterator>   //
// #include <list>

#include <WoodBlock/Core/NamedObject.h>
#include <WoodBlock/Macro.h>
#include <WoodBlock/Namespace.hpp>

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

class DataType : public NamedObject {
  // TODO:
 public:
  DataType(const char* name) : NamedObject(name) {}
};

WOODBLOCK_END_PUBLIC_NAMESPACE
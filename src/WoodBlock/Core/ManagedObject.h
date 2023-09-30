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

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

class ManagedObject {
 public:
  // TODO:
  virtual ~ManagedObject() {}
};

WOODBLOCK_END_PUBLIC_NAMESPACE
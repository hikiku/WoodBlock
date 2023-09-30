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

#include <WoodBlock/Core/ManagedObject.h>

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

class NamedObject : public ManagedObject {
 public:
  NamedObject(const String& name) : ManagedObject(), name(name) {}

  const String& getName() const {
    return name;
  }

 private:
  NamedObject() {}

  String name;
};

WOODBLOCK_END_PUBLIC_NAMESPACE
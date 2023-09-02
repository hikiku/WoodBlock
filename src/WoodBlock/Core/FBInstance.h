// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once
#include <Arduino.h>  // Arduino
// #include <WString.h>  // Arduino
// #include <iterator>   //
// #include <list>

#include <WoodBlock/Core/FBType.h>
#include <WoodBlock/Core/NamedObject.h>
#include <WoodBlock/Macro.h>
#include <WoodBlock/Namespace.hpp>

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

class FBInstance : public NamedObject {
  // TODO:
 public:
  template <class FBTYPE>
  static FBInstance* create(const char* name) {
    FBTYPE* fbType = new FBTYPE();
    if (fbType) {
      return new FBInstance(name, *fbType);
    }
    return nullptr;
  }
  virtual ~FBInstance() {
    delete &fbType;
  }

  FBType& getFBType() {
    return fbType;
  }

 private:
  FBInstance(const char* name, FBType& fbType)
      : NamedObject(name), fbType(fbType) {}

  FBType& fbType;
};

WOODBLOCK_END_PUBLIC_NAMESPACE
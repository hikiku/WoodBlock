// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <WString.h>  // Arduino
#include <iterator>   //
#include <list>

#include <WoodBlock/Macro.h>
#include <WoodBlock/Namespace.hpp>

#include "WoodBlock/DataTypes/DataTypes.h"

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

class InputVariable;
class OutputVariable;

class Variable {
 public:
  Variable(const char* name) : name(name) {}
  virtual ~Variable() {}  // TODO: = 0;

  const String& getName() const {
    return name;
  }
  virtual unsigned int getNumberOfDataType() const = 0;
  virtual DataBox& getDataBox() = 0;

 private:
  String name;
};

class OutputVariable : public Variable {
 public:
  OutputVariable(const char* name) : Variable(name) {}
  // virtual ~OutputVariable() {}

 private:
};

// OutputVariableImpl<SInt>;
template <class TDataBox>
class OutputVariableImpl : public OutputVariable {
 public:
  OutputVariableImpl(const char* name) : OutputVariable(name), dataBox() {}

  unsigned int getNumberOfDataType() const {
    return dataBox.getNumberOfDataType();
  }
  TDataBox& getDataBox() {
    return dataBox;
  }

 private:
  TDataBox dataBox;
};

template <class TDataBox>
using Vo = OutputVariableImpl<TDataBox>;

class InputVariable : public Variable {
 public:
  InputVariable(const char* name) : Variable(name) {}  //, outData(nullptr)
  // virtual ~InputVariable() {}

  // virtual bool sample(OutputVariable& outData) = 0;  // clone data from 'outData'

 protected:
  // OutputVariable* getVariableOutput() {
  //   return outData;
  // }

 private:
  // OutputVariable* outData;
};

// InputVariableImpl<SInt>, Vi<SInt>, ...
template <class TDataBox>
class InputVariableImpl : public InputVariable {
 public:
  InputVariableImpl(const char* name) : InputVariable(name), dataBox() {}

  unsigned int getNumberOfDataType() const {
    return dataBox.getNumberOfDataType();
  }
  TDataBox& getDataBox() {
    return dataBox;
  }

  // // TODO: depended on FBNewwork!!!!!!!!!!!!!!!
  // // FROM: eg: OutputVariableImpl<SInt>, Vi<SInt>, ...
  // // template <class FROM>
  // // bool sample(FROM& outData)  // clone data from 'fromData'
  // bool sample(OutputVariable& outData)
  // {
  //   // OutputVariable *outData = getVariableOutput();
  //   // if (outData)
  //   // {
  //   //     dataBox.copy((TDataBox&)outData->getDataBox()); // TODO:
  //   //     error!!!!!!!!!!!!!!!!!!!!!!!!! return true;
  //   // }

  //   // TODO: only same data type fields may be converted!
  //   // OutputVariableImpl<TDataBox>* outData =
  //   //     (OutputVariableImpl<TDataBox>*)getVariableOutput();
  //     // return dataBox.copy(
  //     //     outData.getDataBox());  // TODO: error!!!!!!!!!!!!!!!!!!!!!!!!!

  //     *(dataBox.getData()) = *(outData.getDataBox().getData());
  //     return true;
  // }

 private:
  TDataBox dataBox;
};

template <class TDataBox>
using Vi = InputVariableImpl<TDataBox>;

class InternalVariable : public Variable {
  // TODO:

  InternalVariable(const char* name) : Variable(name), internalData(nullptr) {}
  // virtual ~InternalVariable() {}

 protected:
 private:
  InternalVariable* internalData;
};

// InternalVariableImpl<SInt>, Vi<SInt>, ...
template <class TDataBox>
class InternalVariableImpl : public InternalVariable {
 public:
  InternalVariableImpl(const char* name) : InternalVariable(name), dataBox() {}

 private:
  TDataBox dataBox;
};

template <class TDataBox>
using Vt = InternalVariableImpl<TDataBox>;

WOODBLOCK_END_PUBLIC_NAMESPACE
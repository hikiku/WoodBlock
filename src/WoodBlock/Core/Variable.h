// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <WString.h>  // Arduino
#include <iterator>   //
#include <list>

#include <WoodBlock/Core/DataType.h>
#include <WoodBlock/Core/Macro.h>
#include <WoodBlock/Namespace.hpp>

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

class VariableInput;
class VariableOutput;

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

class VariableOutput : public Variable {
 public:
  VariableOutput(const char* name) : Variable(name) {}
  // virtual ~VariableOutput() {}

 private:
};

// VariableOutputImpl<SINT>;
template <class TDataBox>
class VariableOutputImpl : public VariableOutput {
 public:
  VariableOutputImpl(const char* name) : VariableOutput(name), dataBox() {}

  unsigned int getNumberOfDataType() const {
    return dataBox.getNumberOfDataType();
  }
  TDataBox& getDataBox() {
    return dataBox;
  }

 private:
  TDataBox dataBox;
};

class VariableInput : public Variable {
 public:
  VariableInput(const char* name) : Variable(name), outData(nullptr) {}
  // virtual ~VariableInput() {}

  bool isAlreadyConnected() const {
    return (outData == nullptr) ? false : true;
  }
  // check that outVariable and inVariable are match
  bool checkForConnectFrom(VariableOutput& outData) const;
  bool connectFrom(VariableOutput& outData) {
    if (!checkForConnectFrom(outData)) {
      return false;
    }
    // connect VariableInput from VariableOutput
    this->outData = &outData;
    return true;
  }
  void disconnect() {
    if (this->outData) {
      this->outData = nullptr;
    }
  }

  virtual bool sample() = 0;  // clone data from 'outData'
 
 protected:
  VariableOutput* getVariableOutput() {
    return outData;
  }

 private:
  VariableOutput* outData;  // start of connection, Output data variable
};

// VariableInputImpl<SINT>;
template <class TDataBox>
class VariableInputImpl : public VariableInput {
 public:
  VariableInputImpl(const char* name) : VariableInput(name), dataBox() {}

  unsigned int getNumberOfDataType() const {
    return dataBox.getNumberOfDataType();
  }
  TDataBox& getDataBox() {
    return dataBox;
  }

  // VariableInputImpl<SINT>;
  // template <class TDataBox>
  bool sample()  // clone data from 'fromData'
  {
    // VariableOutput *outData = getVariableOutput();
    // if (outData)
    // {
    //     dataBox.copy((TDataBox&)outData->getDataBox()); // TODO:
    //     error!!!!!!!!!!!!!!!!!!!!!!!!! return true;
    // }

    // TODO: only same data type fields may be converted!
    VariableOutputImpl<TDataBox>* outData =
        (VariableOutputImpl<TDataBox>*)getVariableOutput();
    if (outData) {
      dataBox.copy(
          outData->getDataBox());  // TODO: error!!!!!!!!!!!!!!!!!!!!!!!!!
      return true;
    }
    return false;
  }

 private:
  TDataBox dataBox;
};

WOODBLOCK_END_PUBLIC_NAMESPACE
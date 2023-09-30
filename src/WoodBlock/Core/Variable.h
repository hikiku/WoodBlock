// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <WString.h>  // Arduino
#include <exception>
#include <iterator>  //
#include <list>

#include <WoodBlock/Macro.h>
#include <WoodBlock/Namespace.hpp>

#include "WoodBlock/Core/NamedObject.h"
#include "WoodBlock/DataTypes/DataTypes.h"

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

class InputVariable;
class OutputVariable;

class Variable : public NamedObject {
 public:
  Variable(const char* name) : NamedObject(name) {}
  virtual ~Variable() {}

  virtual unsigned int getNumberOfDataType() const = 0;
  virtual DataBox& getDataBox() = 0;
};

class OutputVariable : public Variable {
 public:
  OutputVariable(const char* name) : Variable(name) {}
  // virtual ~OutputVariable() {}
};

class InputVariable : public Variable {
 public:
  InputVariable(const char* name) : Variable(name) {}
  // virtual ~InputVariable() {}
};

class InternalVariable : public Variable {
 public:
  InternalVariable(const char* name) : Variable(name) {}
  // virtual ~InternalVariable() {}
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

 private:
  TDataBox dataBox;
};

// InternalVariableImpl<SInt>, Vi<SInt>, ...
template <class TDataBox>
class InternalVariableImpl : public InternalVariable {
 public:
  InternalVariableImpl(const char* name) : InternalVariable(name), dataBox() {}

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

template <class TDataBox>
using Vi = InputVariableImpl<TDataBox>;

template <class TDataBox>
using Vt = InternalVariableImpl<TDataBox>;

class InputVariableException : public std::exception {
  virtual const char* what() const throw() {
    return "Input Variable does not exist!";
  }
};
class OutputVariableException : public std::exception {
  virtual const char* what() const throw() {
    return "Output Variable does not exist!";
  }
};
class InternalVariableException : public std::exception {
  virtual const char* what() const throw() {
    return "Internal Variable does not exist!";
  }
};

WOODBLOCK_END_PUBLIC_NAMESPACE
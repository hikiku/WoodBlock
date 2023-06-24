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

// For user extend data type
extern bool extend_check4ConnectDataType(unsigned int outDataType,
                                         unsigned int inDataType);

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
    return dataBox.getNumber();
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
  static bool check4ConnectDataType(unsigned int outDataType,
                                    unsigned int inDataType) {
    switch (outDataType) {
      // Integer
      case SINT_NO:  // Short integer            8   -128 to þ127
        switch (inDataType) {
          case SINT_NO:  // Short integer            8   -128 to þ127
            return true;
          case INT_NO:  // Integer                  16  -32768 to 32767
            return true;
          case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
            return true;
          case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
            return true;
          case USINT_NO:  // Unsigned short integer   8   0 to 255
            return false;
          case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
            return false;
          case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
            return false;
          case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
            return false;
          case REAL_NO:  // Real numbers         32      +-10^(+-38)
            return true;
          case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
            return true;
          case NO_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
                         // equivalent to 1.
            return false;
          default:
            break;
        }
        break;
      case INT_NO:  // Integer                  16  -32768 to 32767
        switch (inDataType) {
          case SINT_NO:  // Short integer            8   -128 to þ127
            return false;
          case INT_NO:  // Integer                  16  -32768 to 32767
            return true;
          case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
            return true;
          case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
            return true;
          case USINT_NO:  // Unsigned short integer   8   0 to 255
            return false;
          case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
            return false;
          case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
            return false;
          case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
            return false;
          case REAL_NO:  // Real numbers         32      +-10^(+-38)
            return true;
          case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
            return true;
          case NO_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
                         // equivalent to 1.
            return false;
          default:
            break;
        }
        break;
      case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
        switch (inDataType) {
          case SINT_NO:  // Short integer            8   -128 to þ127
            return false;
          case INT_NO:  // Integer                  16  -32768 to 32767
            return false;
          case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
            return true;
          case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
            return true;
          case USINT_NO:  // Unsigned short integer   8   0 to 255
            return false;
          case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
            return false;
          case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
            return false;
          case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
            return false;
          case REAL_NO:  // Real numbers         32      +-10^(+-38)
            return true;
          case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
            return true;
          case NO_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
                         // equivalent to 1.
            return false;
          default:
            break;
        }
        break;
      case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
        switch (inDataType) {
          case SINT_NO:  // Short integer            8   -128 to þ127
            return false;
          case INT_NO:  // Integer                  16  -32768 to 32767
            return false;
          case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
            return false;
          case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
            return true;
          case USINT_NO:  // Unsigned short integer   8   0 to 255
            return false;
          case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
            return false;
          case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
            return false;
          case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
            return false;
          case REAL_NO:  // Real numbers         32      +-10^(+-38)
            return true;
          case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
            return true;
          case NO_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
                         // equivalent to 1.
            return false;
          default:
            break;
        }
        break;
      case USINT_NO:  // Unsigned short integer   8   0 to 255
        switch (inDataType) {
          case SINT_NO:  // Short integer            8   -128 to þ127
            return false;
          case INT_NO:  // Integer                  16  -32768 to 32767
            return true;
          case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
            return true;
          case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
            return true;
          case USINT_NO:  // Unsigned short integer   8   0 to 255
            return true;
          case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
            return true;
          case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
            return true;
          case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
            return true;
          case REAL_NO:  // Real numbers         32      +-10^(+-38)
            return true;
          case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
            return true;
          case NO_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
                         // equivalent to 1.
            return false;
          default:
            break;
        }
        break;
      case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
        switch (inDataType) {
          case SINT_NO:  // Short integer            8   -128 to þ127
            return false;
          case INT_NO:  // Integer                  16  -32768 to 32767
            return false;
          case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
            return true;
          case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
            return true;
          case USINT_NO:  // Unsigned short integer   8   0 to 255
            return false;
          case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
            return true;
          case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
            return true;
          case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
            return true;
          case REAL_NO:  // Real numbers         32      +-10^(+-38)
            return true;
          case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
            return true;
          case NO_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
                         // equivalent to 1.
            return false;
          default:
            break;
        }
        break;
      case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
        switch (inDataType) {
          case SINT_NO:  // Short integer            8   -128 to þ127
            return false;
          case INT_NO:  // Integer                  16  -32768 to 32767
            return false;
          case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
            return false;
          case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
            return true;
          case USINT_NO:  // Unsigned short integer   8   0 to 255
            return false;
          case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
            return false;
          case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
            return true;
          case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
            return true;
          case REAL_NO:  // Real numbers         32      +-10^(+-38)
            return true;
          case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
            return true;
          case NO_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
                         // equivalent to 1.
            return false;
          default:
            break;
        }
        break;
      case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
        switch (inDataType) {
          case SINT_NO:  // Short integer            8   -128 to þ127
            return false;
          case INT_NO:  // Integer                  16  -32768 to 32767
            return false;
          case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
            return false;
          case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
            return false;
          case USINT_NO:  // Unsigned short integer   8   0 to 255
            return false;
          case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
            return false;
          case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
            return false;
          case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
            return true;
          case REAL_NO:  // Real numbers         32      +-10^(+-38)
            return true;
          case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
            return true;
          case NO_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
                         // equivalent to 1.
            return false;
          default:
            break;
        }
        break;
      // Floating point (REAL)
      case REAL_NO:  // Real numbers         32      +-10^(+-38)
        switch (inDataType) {
          case SINT_NO:  // Short integer            8   -128 to þ127
            return false;
          case INT_NO:  // Integer                  16  -32768 to 32767
            return false;
          case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
            return false;
          case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
            return false;
          case USINT_NO:  // Unsigned short integer   8   0 to 255
            return false;
          case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
            return false;
          case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
            return false;
          case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
            return false;
          case REAL_NO:  // Real numbers         32      +-10^(+-38)
            return true;
          case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
            return true;
          case NO_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
                         // equivalent to 1.
            return false;
          default:
            break;
        }
        break;
      case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
        switch (inDataType) {
          case SINT_NO:  // Short integer            8   -128 to þ127
            return false;
          case INT_NO:  // Integer                  16  -32768 to 32767
            return false;
          case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
            return false;
          case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
            return false;
          case USINT_NO:  // Unsigned short integer   8   0 to 255
            return false;
          case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
            return false;
          case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
            return false;
          case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
            return false;
          case REAL_NO:  // Real numbers         32      +-10^(+-38)
            return false;
          case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
            return true;
          case NO_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
                         // equivalent to 1.
            return false;
          default:
            break;
        }
        break;
      case NO_BOOL:  // Long real numbers    64      +-10^(+-308)
        switch (inDataType) {
          case SINT_NO:  // Short integer            8   -128 to þ127
            return false;
          case INT_NO:  // Integer                  16  -32768 to 32767
            return false;
          case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
            return false;
          case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
            return false;
          case USINT_NO:  // Unsigned short integer   8   0 to 255
            return false;
          case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
            return false;
          case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
            return false;
          case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
            return false;
          case REAL_NO:  // Real numbers         32      +-10^(+-38)
            return false;
          case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
            return false;
          case NO_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
                         // equivalent to 1.
            return true;
          default:
            break;
        }
        break;
      default:
        break;
    }

    return extend_check4ConnectDataType(outDataType, inDataType);
  }

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
    return dataBox.getNumber();
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
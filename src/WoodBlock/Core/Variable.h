// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <WString.h>  // Arduino
#include <iterator>   //
#include <list>

#include <WoodBlock/Core/Macro.h>
#include <WoodBlock/Namespace.hpp>

// For user extend data type
extern bool extend_check4ConnectDataType(unsigned int outDataType,
                                         unsigned int inDataType);

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

class VariableInput;
class VariableOutput;

// Integer
typedef int8_t SINT;     // Short integer            8   -128 to þ127
typedef int16_t INT;     // Integer                  16  -32768 to 32767
typedef int32_t DINT;    // Double integer           32  -2^(31) to 2^(31)-1
typedef int64_t LINT;    // Long integer             64  -2^(63) to 2^(63)-1
typedef uint8_t USINT;   // Unsigned short integer   8   0 to 255
typedef uint16_t UINT;   // Unsigned integer         16  0 to 2^(16)-1
typedef uint32_t UDINT;  // Unsigned double integer  32  0 to 2^(32)-1
typedef uint64_t ULINT;  // Unsigned long integer    64  0 to 2^(64)-1
// Floating point (REAL)
typedef float REAL;    // Real numbers         32      +-10^(+-38)
typedef double LREAL;  // Long real numbers    64      +-10^(+-308)

typedef bool
    BOOL;  // Has two states FALSE, equivalent to 0, and TRUE equivalent to 1.

// Data Type // enum class VariableType
// Integer
#define DT_SINT (1)   // Short integer            8   -128 to þ127
#define DT_INT (2)    // Integer                  16  -32768 to 32767
#define DT_DINT (3)   // Double integer           32  -2^(31) to 2^(31)-1
#define DT_LINT (4)   // Long integer             64  -2^(63) to 2^(63)-1
#define DT_USINT (5)  // Unsigned short integer   8   0 to 255
#define DT_UINT (6)   // Unsigned integer         16  0 to 2^(16)-1
#define DT_UDINT (7)  // Unsigned double integer  32  0 to 2^(32)-1
#define DT_ULINT (8)  // Unsigned long integer    64  0 to 2^(64)-1
// Floating point (REAL)
#define DT_REAL (9)    // Real numbers         32      +-10^(+-38)
#define DT_LREAL (10)  // Long real numbers    64      +-10^(+-308)

#define DT_BOOL \
  (30)  // Has two states FALSE, equivalent to 0, and TRUE equivalent to 1.

class DataBox {
 public:
  DataBox(unsigned int dataType, unsigned int dataSize)
      : dataType(dataType), dataSize(dataSize), dataPoint(nullptr) {}
  DataBox(unsigned int dataType, unsigned int dataSize, void* dataPoint)
      : dataType(dataType), dataSize(dataSize), dataPoint(dataPoint) {}
  virtual ~DataBox() {}  // TODO: = 0;

  unsigned int getDataType() const {
    return dataType;
  }
  unsigned int getDataSize() const {
    return dataSize;
  }
  void* getDataPoint() {
    return dataPoint;
  }
  const char* getDataTypeDescription() const {
    switch (dataType) {
      // Integer
      case DT_SINT:  // Short integer            8   -128 to þ127
        return "SINT";
      case DT_INT:  // Integer                  16  -32768 to 32767
        return "INT";
      case DT_DINT:  // Double integer           32  -2^(31) to 2^(31)-1
        return "DINT";
      case DT_LINT:  // Long integer             64  -2^(63) to 2^(63)-1
        return "LINT";
      case DT_USINT:  // Unsigned short integer   8   0 to 255
        return "USINT";
      case DT_UINT:  // Unsigned integer         16  0 to 2^(16)-1
        return "UINT";
      case DT_UDINT:  // Unsigned double integer  32  0 to 2^(32)-1
        return "UDINT";
      case DT_ULINT:  // Unsigned long integer    64  0 to 2^(64)-1
        return "ULINT";
      // Floating point (REAL)
      case DT_REAL:  // Real numbers         32      +-10^(+-38)
        return "REAL";
      case DT_LREAL:  // Long real numbers    64      +-10^(+-308)
        return "LREAL";
      case DT_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
                     // equivalent to 1.
        return "BOOL";
      default:
        return "Unkown";
    }
  }

 protected:
  void setDataPoint(void* dataPoint) {
    this->dataPoint = dataPoint;
  }

 private:
  unsigned int dataType;  // DT_SINT, ...
  unsigned int dataSize;  // bytes
  void* dataPoint;        // data point
};

// eg: DataBoxImpl<SINT, DT_SINT>
template <class T, unsigned int DT>
class DataBoxImpl : public DataBox {
 public:
  DataBoxImpl() : DataBox(DT, sizeof(T)) {
    T* dataPoint = new T();
    setDataPoint(dataPoint);
  }
  ~DataBoxImpl() {
    T* data = getData();
    if (data) {
      delete data;
    }
    setDataPoint(nullptr);
  }

  T* getData() {
    return (T*)getDataPoint();
  }
  void setData(const T& value) {
    T* data = getData();
    if (data) {
      *data = value;
    }
  }

  // FROM: eg: DataBoxSInt or DataBoxImpl<SINT, DT_SINT>
  template <class FROM>
  bool copy(FROM& from)  // const
  {
    T* data = getData();
    if (data && from.getData()) {
      *data = *from.getData();  // TODO: copy assign value function!
      return true;
    } else {
      return false;
    }
  }
};

// Integer
typedef DataBoxImpl<SINT, DT_SINT>
    DataBoxSInt;  // Short integer            8   -128 to þ127
typedef DataBoxImpl<INT, DT_INT>
    DataBoxInt;  // Integer                  16  -32768 to 32767
typedef DataBoxImpl<DINT, DT_DINT>
    DataBoxDInt;  // Double integer           32  -2^(31) to 2^(31)-1
typedef DataBoxImpl<LINT, DT_LINT>
    DataBoxLInt;  // Long integer             64  -2^(63) to 2^(63)-1
typedef DataBoxImpl<USINT, DT_USINT>
    DataBoxUSInt;  // Unsigned short integer   8   0 to 255
typedef DataBoxImpl<UINT, DT_UINT>
    DataBoxUInt;  // Unsigned integer         16  0 to 2^(16)-1
typedef DataBoxImpl<UDINT, DT_UDINT>
    DataBoxUDInt;  // Unsigned double integer  32  0 to 2^(32)-1
typedef DataBoxImpl<ULINT, DT_ULINT>
    DataBoxULInt;  // Unsigned long integer    64  0 to 2^(64)-1
// Floating point (REAL)
typedef DataBoxImpl<REAL, DT_REAL>
    DataBoxReal;  // Real numbers         32      +-10^(+-38)
typedef DataBoxImpl<LREAL, DT_LREAL>
    DataBoxLReal;  // Long real numbers    64      +-10^(+-308)
// BOOL
typedef DataBoxImpl<BOOL, DT_BOOL>
    DataBoxBool;  // Has two states FALSE, equivalent to 0, and TRUE equivalent
                  // to 1.

class Variable {
 public:
  Variable(const char* name) : name(name) {}
  virtual ~Variable() {}  // TODO: = 0;

  const String& getName() const {
    return name;
  }
  virtual unsigned int getDataType() const = 0;
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

// VariableOutputImpl<DataBoxSInt>;
template <class TDataBox>
class VariableOutputImpl : public VariableOutput {
 public:
  VariableOutputImpl(const char* name) : VariableOutput(name), dataBox() {}

  unsigned int getDataType() const {
    return dataBox.getDataType();
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
      case DT_SINT:  // Short integer            8   -128 to þ127
        switch (inDataType) {
          case DT_SINT:  // Short integer            8   -128 to þ127
            return true;
          case DT_INT:  // Integer                  16  -32768 to 32767
            return true;
          case DT_DINT:  // Double integer           32  -2^(31) to 2^(31)-1
            return true;
          case DT_LINT:  // Long integer             64  -2^(63) to 2^(63)-1
            return true;
          case DT_USINT:  // Unsigned short integer   8   0 to 255
            return false;
          case DT_UINT:  // Unsigned integer         16  0 to 2^(16)-1
            return false;
          case DT_UDINT:  // Unsigned double integer  32  0 to 2^(32)-1
            return false;
          case DT_ULINT:  // Unsigned long integer    64  0 to 2^(64)-1
            return false;
          case DT_REAL:  // Real numbers         32      +-10^(+-38)
            return true;
          case DT_LREAL:  // Long real numbers    64      +-10^(+-308)
            return true;
          case DT_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
                         // equivalent to 1.
            return false;
          default:
            break;
        }
        break;
      case DT_INT:  // Integer                  16  -32768 to 32767
        switch (inDataType) {
          case DT_SINT:  // Short integer            8   -128 to þ127
            return false;
          case DT_INT:  // Integer                  16  -32768 to 32767
            return true;
          case DT_DINT:  // Double integer           32  -2^(31) to 2^(31)-1
            return true;
          case DT_LINT:  // Long integer             64  -2^(63) to 2^(63)-1
            return true;
          case DT_USINT:  // Unsigned short integer   8   0 to 255
            return false;
          case DT_UINT:  // Unsigned integer         16  0 to 2^(16)-1
            return false;
          case DT_UDINT:  // Unsigned double integer  32  0 to 2^(32)-1
            return false;
          case DT_ULINT:  // Unsigned long integer    64  0 to 2^(64)-1
            return false;
          case DT_REAL:  // Real numbers         32      +-10^(+-38)
            return true;
          case DT_LREAL:  // Long real numbers    64      +-10^(+-308)
            return true;
          case DT_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
                         // equivalent to 1.
            return false;
          default:
            break;
        }
        break;
      case DT_DINT:  // Double integer           32  -2^(31) to 2^(31)-1
        switch (inDataType) {
          case DT_SINT:  // Short integer            8   -128 to þ127
            return false;
          case DT_INT:  // Integer                  16  -32768 to 32767
            return false;
          case DT_DINT:  // Double integer           32  -2^(31) to 2^(31)-1
            return true;
          case DT_LINT:  // Long integer             64  -2^(63) to 2^(63)-1
            return true;
          case DT_USINT:  // Unsigned short integer   8   0 to 255
            return false;
          case DT_UINT:  // Unsigned integer         16  0 to 2^(16)-1
            return false;
          case DT_UDINT:  // Unsigned double integer  32  0 to 2^(32)-1
            return false;
          case DT_ULINT:  // Unsigned long integer    64  0 to 2^(64)-1
            return false;
          case DT_REAL:  // Real numbers         32      +-10^(+-38)
            return true;
          case DT_LREAL:  // Long real numbers    64      +-10^(+-308)
            return true;
          case DT_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
                         // equivalent to 1.
            return false;
          default:
            break;
        }
        break;
      case DT_LINT:  // Long integer             64  -2^(63) to 2^(63)-1
        switch (inDataType) {
          case DT_SINT:  // Short integer            8   -128 to þ127
            return false;
          case DT_INT:  // Integer                  16  -32768 to 32767
            return false;
          case DT_DINT:  // Double integer           32  -2^(31) to 2^(31)-1
            return false;
          case DT_LINT:  // Long integer             64  -2^(63) to 2^(63)-1
            return true;
          case DT_USINT:  // Unsigned short integer   8   0 to 255
            return false;
          case DT_UINT:  // Unsigned integer         16  0 to 2^(16)-1
            return false;
          case DT_UDINT:  // Unsigned double integer  32  0 to 2^(32)-1
            return false;
          case DT_ULINT:  // Unsigned long integer    64  0 to 2^(64)-1
            return false;
          case DT_REAL:  // Real numbers         32      +-10^(+-38)
            return true;
          case DT_LREAL:  // Long real numbers    64      +-10^(+-308)
            return true;
          case DT_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
                         // equivalent to 1.
            return false;
          default:
            break;
        }
        break;
      case DT_USINT:  // Unsigned short integer   8   0 to 255
        switch (inDataType) {
          case DT_SINT:  // Short integer            8   -128 to þ127
            return false;
          case DT_INT:  // Integer                  16  -32768 to 32767
            return true;
          case DT_DINT:  // Double integer           32  -2^(31) to 2^(31)-1
            return true;
          case DT_LINT:  // Long integer             64  -2^(63) to 2^(63)-1
            return true;
          case DT_USINT:  // Unsigned short integer   8   0 to 255
            return true;
          case DT_UINT:  // Unsigned integer         16  0 to 2^(16)-1
            return true;
          case DT_UDINT:  // Unsigned double integer  32  0 to 2^(32)-1
            return true;
          case DT_ULINT:  // Unsigned long integer    64  0 to 2^(64)-1
            return true;
          case DT_REAL:  // Real numbers         32      +-10^(+-38)
            return true;
          case DT_LREAL:  // Long real numbers    64      +-10^(+-308)
            return true;
          case DT_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
                         // equivalent to 1.
            return false;
          default:
            break;
        }
        break;
      case DT_UINT:  // Unsigned integer         16  0 to 2^(16)-1
        switch (inDataType) {
          case DT_SINT:  // Short integer            8   -128 to þ127
            return false;
          case DT_INT:  // Integer                  16  -32768 to 32767
            return false;
          case DT_DINT:  // Double integer           32  -2^(31) to 2^(31)-1
            return true;
          case DT_LINT:  // Long integer             64  -2^(63) to 2^(63)-1
            return true;
          case DT_USINT:  // Unsigned short integer   8   0 to 255
            return false;
          case DT_UINT:  // Unsigned integer         16  0 to 2^(16)-1
            return true;
          case DT_UDINT:  // Unsigned double integer  32  0 to 2^(32)-1
            return true;
          case DT_ULINT:  // Unsigned long integer    64  0 to 2^(64)-1
            return true;
          case DT_REAL:  // Real numbers         32      +-10^(+-38)
            return true;
          case DT_LREAL:  // Long real numbers    64      +-10^(+-308)
            return true;
          case DT_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
                         // equivalent to 1.
            return false;
          default:
            break;
        }
        break;
      case DT_UDINT:  // Unsigned double integer  32  0 to 2^(32)-1
        switch (inDataType) {
          case DT_SINT:  // Short integer            8   -128 to þ127
            return false;
          case DT_INT:  // Integer                  16  -32768 to 32767
            return false;
          case DT_DINT:  // Double integer           32  -2^(31) to 2^(31)-1
            return false;
          case DT_LINT:  // Long integer             64  -2^(63) to 2^(63)-1
            return true;
          case DT_USINT:  // Unsigned short integer   8   0 to 255
            return false;
          case DT_UINT:  // Unsigned integer         16  0 to 2^(16)-1
            return false;
          case DT_UDINT:  // Unsigned double integer  32  0 to 2^(32)-1
            return true;
          case DT_ULINT:  // Unsigned long integer    64  0 to 2^(64)-1
            return true;
          case DT_REAL:  // Real numbers         32      +-10^(+-38)
            return true;
          case DT_LREAL:  // Long real numbers    64      +-10^(+-308)
            return true;
          case DT_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
                         // equivalent to 1.
            return false;
          default:
            break;
        }
        break;
      case DT_ULINT:  // Unsigned long integer    64  0 to 2^(64)-1
        switch (inDataType) {
          case DT_SINT:  // Short integer            8   -128 to þ127
            return false;
          case DT_INT:  // Integer                  16  -32768 to 32767
            return false;
          case DT_DINT:  // Double integer           32  -2^(31) to 2^(31)-1
            return false;
          case DT_LINT:  // Long integer             64  -2^(63) to 2^(63)-1
            return false;
          case DT_USINT:  // Unsigned short integer   8   0 to 255
            return false;
          case DT_UINT:  // Unsigned integer         16  0 to 2^(16)-1
            return false;
          case DT_UDINT:  // Unsigned double integer  32  0 to 2^(32)-1
            return false;
          case DT_ULINT:  // Unsigned long integer    64  0 to 2^(64)-1
            return true;
          case DT_REAL:  // Real numbers         32      +-10^(+-38)
            return true;
          case DT_LREAL:  // Long real numbers    64      +-10^(+-308)
            return true;
          case DT_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
                         // equivalent to 1.
            return false;
          default:
            break;
        }
        break;
      // Floating point (REAL)
      case DT_REAL:  // Real numbers         32      +-10^(+-38)
        switch (inDataType) {
          case DT_SINT:  // Short integer            8   -128 to þ127
            return false;
          case DT_INT:  // Integer                  16  -32768 to 32767
            return false;
          case DT_DINT:  // Double integer           32  -2^(31) to 2^(31)-1
            return false;
          case DT_LINT:  // Long integer             64  -2^(63) to 2^(63)-1
            return false;
          case DT_USINT:  // Unsigned short integer   8   0 to 255
            return false;
          case DT_UINT:  // Unsigned integer         16  0 to 2^(16)-1
            return false;
          case DT_UDINT:  // Unsigned double integer  32  0 to 2^(32)-1
            return false;
          case DT_ULINT:  // Unsigned long integer    64  0 to 2^(64)-1
            return false;
          case DT_REAL:  // Real numbers         32      +-10^(+-38)
            return true;
          case DT_LREAL:  // Long real numbers    64      +-10^(+-308)
            return true;
          case DT_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
                         // equivalent to 1.
            return false;
          default:
            break;
        }
        break;
      case DT_LREAL:  // Long real numbers    64      +-10^(+-308)
        switch (inDataType) {
          case DT_SINT:  // Short integer            8   -128 to þ127
            return false;
          case DT_INT:  // Integer                  16  -32768 to 32767
            return false;
          case DT_DINT:  // Double integer           32  -2^(31) to 2^(31)-1
            return false;
          case DT_LINT:  // Long integer             64  -2^(63) to 2^(63)-1
            return false;
          case DT_USINT:  // Unsigned short integer   8   0 to 255
            return false;
          case DT_UINT:  // Unsigned integer         16  0 to 2^(16)-1
            return false;
          case DT_UDINT:  // Unsigned double integer  32  0 to 2^(32)-1
            return false;
          case DT_ULINT:  // Unsigned long integer    64  0 to 2^(64)-1
            return false;
          case DT_REAL:  // Real numbers         32      +-10^(+-38)
            return false;
          case DT_LREAL:  // Long real numbers    64      +-10^(+-308)
            return true;
          case DT_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
                         // equivalent to 1.
            return false;
          default:
            break;
        }
        break;
      case DT_BOOL:  // Long real numbers    64      +-10^(+-308)
        switch (inDataType) {
          case DT_SINT:  // Short integer            8   -128 to þ127
            return false;
          case DT_INT:  // Integer                  16  -32768 to 32767
            return false;
          case DT_DINT:  // Double integer           32  -2^(31) to 2^(31)-1
            return false;
          case DT_LINT:  // Long integer             64  -2^(63) to 2^(63)-1
            return false;
          case DT_USINT:  // Unsigned short integer   8   0 to 255
            return false;
          case DT_UINT:  // Unsigned integer         16  0 to 2^(16)-1
            return false;
          case DT_UDINT:  // Unsigned double integer  32  0 to 2^(32)-1
            return false;
          case DT_ULINT:  // Unsigned long integer    64  0 to 2^(64)-1
            return false;
          case DT_REAL:  // Real numbers         32      +-10^(+-38)
            return false;
          case DT_LREAL:  // Long real numbers    64      +-10^(+-308)
            return false;
          case DT_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
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

// VariableInputImpl<DataBoxSInt>;
template <class TDataBox>
class VariableInputImpl : public VariableInput {
 public:
  VariableInputImpl(const char* name) : VariableInput(name), dataBox() {}

  unsigned int getDataType() const {
    return dataBox.getDataType();
  }
  TDataBox& getDataBox() {
    return dataBox;
  }

  // VariableInputImpl<DataBoxSInt>;
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

// Integer
// typedef VariableInputImpl<DataBoxSInt> VarInSInt; // Short integer 8   -128
// to þ127 typedef DataBoxImpl<INT, DT_INT> DataBoxInt;       // Integer 16
// -32768 to 32767 typedef DataBoxImpl<DINT, DT_DINT> DataBoxDInt;    // Double
// integer           32  -2^(31) to 2^(31)-1 typedef DataBoxImpl<LINT, DT_LINT>
// DataBoxLInt;    // Long integer             64  -2^(63) to 2^(63)-1 typedef
// DataBoxImpl<USINT, DT_USINT> DataBoxUSInt; // Unsigned short integer   8   0
// to 255 typedef DataBoxImpl<UINT, DT_UINT> DataBoxUInt;    // Unsigned integer
// 16  0 to 2^(16)-1 typedef DataBoxImpl<UDINT, DT_UDINT> DataBoxUDInt; //
// Unsigned double integer  32  0 to 2^(32)-1 typedef DataBoxImpl<ULINT,
// DT_ULINT> DataBoxULInt; // Unsigned long integer    64  0 to 2^(64)-1
// // Floating point (REAL)
// typedef DataBoxImpl<REAL, DT_REAL> DataBoxReal;    // Real numbers         32
// +-10^(+-38) typedef DataBoxImpl<LREAL, DT_LREAL> DataBoxLReal; // Long real
// numbers    64      +-10^(+-308) BOOL typedef VariableInputImpl<DataBoxBool>
// VarInBool; // Has two states FALSE, equivalent to 0, and TRUE equivalent
// to 1.

WOODBLOCK_END_PUBLIC_NAMESPACE
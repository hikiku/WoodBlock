// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <WoodBlock/Core/Macro.h>
#include <stdint.h>
#include <WoodBlock/Namespace.hpp>

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

// Integer
typedef int8_t SINT_T;     // Short integer            8   -128 to þ127
typedef int16_t INT_T;     // Integer                  16  -32768 to 32767
typedef int32_t DINT_T;    // Double integer           32  -2^(31) to 2^(31)-1
typedef int64_t LINT_T;    // Long integer             64  -2^(63) to 2^(63)-1
typedef uint8_t USINT_T;   // Unsigned short integer   8   0 to 255
typedef uint16_t UINT_T;   // Unsigned integer         16  0 to 2^(16)-1
typedef uint32_t UDINT_T;  // Unsigned double integer  32  0 to 2^(32)-1
typedef uint64_t ULINT_T;  // Unsigned long integer    64  0 to 2^(64)-1

// Floating point (REAL)
typedef float REAL_T;    // Real numbers         32      +-10^(+-38)
typedef double LREAL_T;  // Long real numbers    64      +-10^(+-308)

typedef bool
    BOOL_T;  // Has two states FALSE, equivalent to 0, and TRUE equivalent to 1.

// Data Type // enum class VariableType
// Integer
#define SINT_NO (1)   // Short integer            8   -128 to þ127
#define INT_NO (2)    // Integer                  16  -32768 to 32767
#define DINT_NO (3)   // Double integer           32  -2^(31) to 2^(31)-1
#define LINT_NO (4)   // Long integer             64  -2^(63) to 2^(63)-1
#define USINT_NO (5)  // Unsigned short integer   8   0 to 255
#define UINT_NO (6)   // Unsigned integer         16  0 to 2^(16)-1
#define UDINT_NO (7)  // Unsigned double integer  32  0 to 2^(32)-1
#define ULINT_NO (8)  // Unsigned long integer    64  0 to 2^(64)-1
// Floating point (REAL)
#define REAL_NO (9)    // Real numbers         32      +-10^(+-38)
#define LREAL_NO (10)  // Long real numbers    64      +-10^(+-308)

#define NO_BOOL \
  (30)  // Has two states FALSE, equivalent to 0, and TRUE equivalent to 1.

class DataBox {
 public:
  DataBox(unsigned int dataType, unsigned int dataSize)
      : dataType(dataType), dataSize(dataSize), dataPoint(nullptr) {}
  DataBox(unsigned int dataType, unsigned int dataSize, void* dataPoint)
      : dataType(dataType), dataSize(dataSize), dataPoint(dataPoint) {}
  virtual ~DataBox() {}  // TODO: = 0;

  unsigned int getNumber() const {
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
      case SINT_NO:  // Short integer            8   -128 to þ127
        return "SINT";
      case INT_NO:  // Integer                  16  -32768 to 32767
        return "INT";
      case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
        return "DINT";
      case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
        return "LINT";
      case USINT_NO:  // Unsigned short integer   8   0 to 255
        return "USINT";
      case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
        return "UINT";
      case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
        return "UDINT";
      case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
        return "ULINT";
      // Floating point (REAL)
      case REAL_NO:  // Real numbers         32      +-10^(+-38)
        return "REAL";
      case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
        return "LREAL";
      case NO_BOOL:  // Has two states FALSE, equivalent to 0, and TRUE
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
  unsigned int dataType;  // SINT_NO, ...
  unsigned int dataSize;  // bytes
  void* dataPoint;        // data point
};

// eg: DataBoxImpl<SINT_T, SINT_NO>
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

  // FROM: eg: SINT or DataBoxImpl<SINT_T, SINT_NO>
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
typedef DataBoxImpl<SINT_T, SINT_NO>
    SINT;  // Short integer            8   -128 to þ127
typedef DataBoxImpl<INT_T, INT_NO>
    INT;  // Integer                  16  -32768 to 32767
typedef DataBoxImpl<DINT_T, DINT_NO>
    DINT;  // Double integer           32  -2^(31) to 2^(31)-1
typedef DataBoxImpl<LINT_T, LINT_NO>
    LINT;  // Long integer             64  -2^(63) to 2^(63)-1
typedef DataBoxImpl<USINT_T, USINT_NO>
    USINT;  // Unsigned short integer   8   0 to 255
typedef DataBoxImpl<UINT_T, UINT_NO>
    UINT;  // Unsigned integer         16  0 to 2^(16)-1
typedef DataBoxImpl<UDINT_T, UDINT_NO>
    UDINT;  // Unsigned double integer  32  0 to 2^(32)-1
typedef DataBoxImpl<ULINT_T, ULINT_NO>
    ULINT;  // Unsigned long integer    64  0 to 2^(64)-1
// Floating point (REAL)
typedef DataBoxImpl<REAL_T, REAL_NO>
    REAL;  // Real numbers         32      +-10^(+-38)
typedef DataBoxImpl<LREAL_T, LREAL_NO>
    LREAL;  // Long real numbers    64      +-10^(+-308)
// BOOL
typedef DataBoxImpl<BOOL_T, NO_BOOL>
    BOOL;  // Has two states FALSE, equivalent to 0, and TRUE equivalent
                  // to 1.

// Integer
// typedef VariableInputImpl<SINT> VarInSInt; // Short integer 8   -128
// to þ127 typedef DataBoxImpl<INT_T, INT_NO> INT;       // Integer 16
// -32768 to 32767 typedef DataBoxImpl<DINT_T, DINT_NO> DINT;    // Double
// integer           32  -2^(31) to 2^(31)-1 typedef DataBoxImpl<LINT_T, LINT_NO>
// LINT;    // Long integer             64  -2^(63) to 2^(63)-1 typedef
// DataBoxImpl<USINT_T, USINT_NO> USINT; // Unsigned short integer   8   0
// to 255 typedef DataBoxImpl<UINT_T, SINT_NO> UINT;    // Unsigned integer
// 16  0 to 2^(16)-1 typedef DataBoxImpl<UDINT_T, UDINT_NO> UDINT; //
// Unsigned double integer  32  0 to 2^(32)-1 typedef DataBoxImpl<ULINT_T,
// ULINT_NO> ULINT; // Unsigned long integer    64  0 to 2^(64)-1
// // Floating point (REAL)
// typedef DataBoxImpl<REAL_T, REAL_NO> REAL;    // Real numbers         32
// +-10^(+-38) typedef DataBoxImpl<LREAL_T, LREAL_NO> LREAL; // Long real
// numbers    64      +-10^(+-308) BOOL_T typedef VariableInputImpl<BOOL>
// VarInBool; // Has two states FALSE, equivalent to 0, and TRUE equivalent
// to 1.

WOODBLOCK_END_PUBLIC_NAMESPACE
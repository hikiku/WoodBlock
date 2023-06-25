// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <WString.h>  // Arduino
#include <WoodBlock/Core/Macro.h>
#include <stdint.h>
#include <WoodBlock/Namespace.hpp>

// For user extend data type
extern bool extend_check4ConnectDataType(unsigned int outNumberOfDataType,
                                         unsigned int inNumberOfDataType);

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

class DataBox {
 public:
  DataBox(const char* name, size_t size)            //, unsigned int number
      : name(name), size(size), dp(nullptr) {}      //, number(number)
  DataBox(const char* name, size_t size, void* dp)  //, unsigned int number
      : name(name), size(size), dp(dp) {}           //, number(number)
  virtual ~DataBox() {}                             // TODO: = 0;

  static bool check4ConnectDataType(unsigned int outNumberOfDataType,
                                    unsigned int inNumberOfDataType);
  // return name of data type
  const char* getNameOfDataType() const {
    return name;
    // switch (numberOfDataType) {
    //   // Integer
    //   case SINT_NO:  // Short integer            8   -128 to þ127
    //     return "SINT";
    //   case INT_NO:  // Integer                  16  -32768 to 32767
    //     return "INT";
    //   case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
    //     return "DINT";
    //   case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
    //     return "LINT";
    //   case USINT_NO:  // Unsigned short integer   8   0 to 255
    //     return "USINT";
    //   case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
    //     return "UINT";
    //   case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
    //     return "UDINT";
    //   case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
    //     return "ULINT";
    //   // Floating point (REAL)
    //   case REAL_NO:  // Real numbers         32      +-10^(+-38)
    //     return "REAL";
    //   case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
    //     return "LREAL";
    //   case BOOL_NO:  // Has two states FALSE, equivalent to 0, and TRUE
    //                  // equivalent to 1.
    //     return "BOOL";
    //   default:
    //     return "Unkown";
    // }
  }

  // return sizeof data type
  size_t getSizeOfDataType() const {
    return size;
  }

  // return number of data type
  unsigned int getNumberOfDataType() const {
    return (unsigned int)name;
    // return number;
  }

  void* getDataPoint() {
    return dp;
  }

 protected:
  void setDataPoint(void* dp) {
    this->dp = dp;
  }

 private:
  const char* name;
  size_t size;  // bytes
  // unsigned int number;  // number of data type, SINT_NO, ...
  void* dp;  // data point
};

// eg: DataBoxImpl<SINT_T, "SINT"> //, SINT_NO
template <class T, const char* nameOfT>  //, unsigned int DT_NO
class DataBoxImpl : public DataBox {
 public:
  DataBoxImpl() : DataBox(nameOfT, sizeof(T)) {  //, DT_NO
    T* data = new T();
    setDataPoint(data);
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

  // FROM: eg: SINT or DataBoxImpl<SINT_T, "SINT"> //, SINT_NO
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

// // Data Type // enum class VariableType
// // Integer
// #define SINT_NO (1)   // Short integer            8   -128 to þ127
// #define INT_NO (2)    // Integer                  16  -32768 to 32767
// #define DINT_NO (3)   // Double integer           32  -2^(31) to 2^(31)-1
// #define LINT_NO (4)   // Long integer             64  -2^(63) to 2^(63)-1
// #define USINT_NO (5)  // Unsigned short integer   8   0 to 255
// #define UINT_NO (6)   // Unsigned integer         16  0 to 2^(16)-1
// #define UDINT_NO (7)  // Unsigned double integer  32  0 to 2^(32)-1
// #define ULINT_NO (8)  // Unsigned long integer    64  0 to 2^(64)-1
// // Floating point (REAL)
// #define REAL_NO (9)    // Real numbers         32      +-10^(+-38)
// #define LREAL_NO (10)  // Long real numbers    64      +-10^(+-308)

// #define BOOL_NO    (30)  // Has two states FALSE, equivalent to 0, and TRUE
// equivalent to 1.

// struct SintType {
//   static const char* name;
//   static const unsigned int number;
// };

// const char* SintType::name = "SINT";
// const unsigned int SintType::number = SINT_NO;

// struct BoolType {
//   static const char* name;
//   static const unsigned int number;
// };

// const char* BoolType::name = "BOOL";
// const unsigned int BoolType::number = BOOL_NO;

extern const char BoolType_name[];
// const char* BoolType_name = "BOOL";

// const unsigned int BoolType_ID =
//     BOOL_NO;  //(const unsigned int)&BoolType_name[0]; //BOOL_NO;

// Integer
// typedef DataBoxImpl<SINT_T, SintType::name, SintType::number>
//     SINT;  // Short integer            8   -128 to þ127
// typedef DataBoxImpl<INT_T, "INT", INT_NO>
//     INT;  // Integer                  16  -32768 to 32767
// typedef DataBoxImpl<DINT_T, "DINT", DINT_NO>
//     DINT;  // Double integer           32  -2^(31) to 2^(31)-1
// typedef DataBoxImpl<LINT_T, "LINT", LINT_NO>
//     LINT;  // Long integer             64  -2^(63) to 2^(63)-1
// typedef DataBoxImpl<USINT_T, "USINT", USINT_NO>
//     USINT;  // Unsigned short integer   8   0 to 255
// typedef DataBoxImpl<UINT_T, "UINT", UINT_NO>
//     UINT;  // Unsigned integer         16  0 to 2^(16)-1
// typedef DataBoxImpl<UDINT_T, "UDINT", UDINT_NO>
//     UDINT;  // Unsigned double integer  32  0 to 2^(32)-1
// typedef DataBoxImpl<ULINT_T, "ULINT", ULINT_NO>
//     ULINT;  // Unsigned long integer    64  0 to 2^(64)-1
// // Floating point (REAL)
// typedef DataBoxImpl<REAL_T, "REAL", REAL_NO>
//     REAL;  // Real numbers         32      +-10^(+-38)
// typedef DataBoxImpl<LREAL_T, "LREAL", LREAL_NO>
//     LREAL;  // Long real numbers    64      +-10^(+-308)
// BOOL
typedef DataBoxImpl<BOOL_T, BoolType_name>  // BOOL_NO, BoolType_ID
    BOOL;                                   // Has two states FALSE, equivalent
                                            // to 0, and TRUE equivalent to 1.

// Integer
// typedef VariableInputImpl<SINT> VarInSInt; // Short integer 8   -128 to þ127
// typedef DataBoxImpl<INT_T, INT_NO> INT;       // Integer 16 -32768 to 32767
// typedef DataBoxImpl<DINT_T, DINT_NO> DINT;    // Double integer           32
// -2^(31) to 2^(31)-1 typedef DataBoxImpl<LINT_T, LINT_NO> // LINT; // Long
// integer             64  -2^(63) to 2^(63)-1 typedef DataBoxImpl<USINT_T,
// USINT_NO> USINT; // Unsigned short integer   8   0 to 255 typedef
// DataBoxImpl<UINT_T, SINT_NO> UINT;    // Unsigned integer 16  0 to 2^(16)-1
// typedef DataBoxImpl<UDINT_T, UDINT_NO> UDINT; // Unsigned double integer  32
// 0 to 2^(32)-1 typedef DataBoxImpl<ULINT_T, ULINT_NO> ULINT; // Unsigned long
// integer    64  0 to 2^(64)-1
// // Floating point (REAL)
// typedef DataBoxImpl<REAL_T, REAL_NO> REAL;    // Real numbers         32
// +-10^(+-38) typedef DataBoxImpl<LREAL_T, LREAL_NO> LREAL; // Long real
// numbers    64      +-10^(+-308) BOOL_T typedef VariableInputImpl<BOOL>
// VarInBool; // Has two states FALSE, equivalent to 0, and TRUE equivalent
// to 1.

WOODBLOCK_END_PUBLIC_NAMESPACE
// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <stddef.h>
// #include <WString.h>  // Arduino

#include <WoodBlock/Namespace.hpp>
// #include <WoodBlock/Macro.h>

// For user extend data type
extern bool extend_check4ConnectDataType(unsigned int outNumberOfDataType,
                                         unsigned int inNumberOfDataType);

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

class DataBox {
 public:
  DataBox(const char* name, size_t size)
      : name(name), size(size), dp(nullptr) {}
  DataBox(const char* name, size_t size, void* dp)
      : name(name), size(size), dp(dp) {}
  virtual ~DataBox() {}

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
    //     return "Int";
    //   case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
    //     return "DInt";
    //   case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
    //     return "LInt";
    //   case USINT_NO:  // Unsigned short integer   8   0 to 255
    //     return "USInt";
    //   case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
    //     return "UInt";
    //   case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
    //     return "UDInt";
    //   case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
    //     return "ULInt";
    //   // Floating point (Real)
    //   case REAL_NO:  // Real numbers         32      +-10^(+-38)
    //     return "Real";
    //   case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
    //     return "LReal";
    //   case BOOL_NO:  // Has two states FALSE, equivalent to 0, and TRUE
    //                  // equivalent to 1.
    //     return "Bool";
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
  void* dp;  // data point
};

// eg: DataBoxImpl<SINT, "SINT">
template <class T, const char* nameOfT>
class DataBoxImpl : public DataBox {
 public:
  DataBoxImpl() : DataBox(nameOfT, sizeof(T)) {
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

  // // FROM: eg: SInt
  // template <class FROM>
  // bool copy(FROM& from)  // const
  // {
  //   T* data = getData();
  //   if (data && from.getData()) {
  //     *data = *from.getData();  // TODO: copy assign value function!
  //     return true;
  //   } else {
  //     return false;
  //   }
  // }
};

WOODBLOCK_END_PUBLIC_NAMESPACE
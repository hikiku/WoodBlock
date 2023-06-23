// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#ifndef WoodData_class_h
#define WoodData_class_h
#ifdef __cplusplus

#include <list>
#include <iterator> //

#include <WString.h>

#include "WoodMacro.h"

class WoodInData;
class WoodOutData;

// Integer
typedef int8_t SINT;    // Short integer            8   -128 to þ127
typedef int16_t INT;    // Integer                  16  -32768 to 32767
typedef int32_t DINT;   // Double integer           32  -2^(31) to 2^(31)-1
typedef int64_t LINT;   // Long integer             64  -2^(63) to 2^(63)-1
typedef uint8_t USINT;  // Unsigned short integer   8   0 to 255
typedef uint16_t UINT;  // Unsigned integer         16  0 to 2^(16)-1
typedef uint32_t UDINT; // Unsigned double integer  32  0 to 2^(32)-1
typedef uint64_t ULINT; // Unsigned long integer    64  0 to 2^(64)-1
// Floating point (REAL)
typedef float REAL;   // Real numbers         32      +-10^(+-38)
typedef double LREAL; // Long real numbers    64      +-10^(+-308)

typedef bool BOOL; // Has two states FALSE, equivalent to 0, and TRUE equivalent to 1.

// Wood Data Type // enum class WoodDataType
// Integer
#define DT_SINT (1)  // Short integer            8   -128 to þ127
#define DT_INT (2)   // Integer                  16  -32768 to 32767
#define DT_DINT (3)  // Double integer           32  -2^(31) to 2^(31)-1
#define DT_LINT (4)  // Long integer             64  -2^(63) to 2^(63)-1
#define DT_USINT (5) // Unsigned short integer   8   0 to 255
#define DT_UINT (6)  // Unsigned integer         16  0 to 2^(16)-1
#define DT_UDINT (7) // Unsigned double integer  32  0 to 2^(32)-1
#define DT_ULINT (8) // Unsigned long integer    64  0 to 2^(64)-1
// Floating point (REAL)
#define DT_REAL (9)   // Real numbers         32      +-10^(+-38)
#define DT_LREAL (10) // Long real numbers    64      +-10^(+-308)

#define DT_BOOL (30) // Has two states FALSE, equivalent to 0, and TRUE equivalent to 1.

class WoodDataBox
{
public:
    WoodDataBox(unsigned int dataType, unsigned int dataSize)
        : dataType(dataType), dataSize(dataSize), dataPoint(nullptr) {}
    WoodDataBox(unsigned int dataType, unsigned int dataSize, void *dataPoint)
        : dataType(dataType), dataSize(dataSize), dataPoint(dataPoint) {}
    virtual ~WoodDataBox() {} // TODO: = 0;

    unsigned int getDataType() const { return dataType; }
    unsigned int getDataSize() const { return dataSize; }
    void *getDataPoint() { return dataPoint; }
    const char *getDataTypeDescription() const
    {
        switch (dataType)
        {
        // Integer
        case DT_SINT: // Short integer            8   -128 to þ127
            return "SINT";
        case DT_INT: // Integer                  16  -32768 to 32767
            return "INT";
        case DT_DINT: // Double integer           32  -2^(31) to 2^(31)-1
            return "DINT";
        case DT_LINT: // Long integer             64  -2^(63) to 2^(63)-1
            return "LINT";
        case DT_USINT: // Unsigned short integer   8   0 to 255
            return "USINT";
        case DT_UINT: // Unsigned integer         16  0 to 2^(16)-1
            return "UINT";
        case DT_UDINT: // Unsigned double integer  32  0 to 2^(32)-1
            return "UDINT";
        case DT_ULINT: // Unsigned long integer    64  0 to 2^(64)-1
            return "ULINT";
        // Floating point (REAL)
        case DT_REAL: // Real numbers         32      +-10^(+-38)
            return "REAL";
        case DT_LREAL: // Long real numbers    64      +-10^(+-308)
            return "LREAL";
        case DT_BOOL: // Has two states FALSE, equivalent to 0, and TRUE equivalent to 1.
            return "BOOL";
        default:
            return "Unkown";
        }
    }

protected:
    void setDataPoint(void *dataPoint) { this->dataPoint = dataPoint; }

private:
    unsigned int dataType; // DT_SINT, ...
    unsigned int dataSize; // bytes
    void *dataPoint;       // data point
};

// eg: WoodDataBoxImpl<SINT, DT_SINT>
template <class T, unsigned int DT>
class WoodDataBoxImpl : public WoodDataBox
{
public:
    WoodDataBoxImpl()
        : WoodDataBox(DT, sizeof(T))
    {
        T *dataPoint = new T();
        setDataPoint(dataPoint);
    }
    ~WoodDataBoxImpl()
    {
        T *data = getData();
        if (data)
        {
            delete data;
        }
        setDataPoint(nullptr);
    }

    T *getData() { return (T *)getDataPoint(); }
    void setData(const T &value)
    {
        T *data = getData();
        if (data)
        {
            *data = value;
        }
    }

    // FROM: eg: WoodSIntDataBox or WoodDataBoxImpl<SINT, DT_SINT>
    template <class FROM>
    bool copy(FROM &from) // const
    {
        T *data = getData();
        if (data && from.getData())
        {
            *data = *from.getData(); // TODO: copy assign value function!
            return true;
        }
        else
        {
            return false;
        }
    }
};

// Integer
typedef WoodDataBoxImpl<SINT, DT_SINT> WoodSIntDataBox;    // Short integer            8   -128 to þ127
typedef WoodDataBoxImpl<INT, DT_INT> WoodIntDataBox;       // Integer                  16  -32768 to 32767
typedef WoodDataBoxImpl<DINT, DT_DINT> WoodDIntDataBox;    // Double integer           32  -2^(31) to 2^(31)-1
typedef WoodDataBoxImpl<LINT, DT_LINT> WoodLIntDataBox;    // Long integer             64  -2^(63) to 2^(63)-1
typedef WoodDataBoxImpl<USINT, DT_USINT> WoodUSIntDataBox; // Unsigned short integer   8   0 to 255
typedef WoodDataBoxImpl<UINT, DT_UINT> WoodUIntDataBox;    // Unsigned integer         16  0 to 2^(16)-1
typedef WoodDataBoxImpl<UDINT, DT_UDINT> WoodUDIntDataBox; // Unsigned double integer  32  0 to 2^(32)-1
typedef WoodDataBoxImpl<ULINT, DT_ULINT> WoodULIntDataBox; // Unsigned long integer    64  0 to 2^(64)-1
// Floating point (REAL)
typedef WoodDataBoxImpl<REAL, DT_REAL> WoodRealDataBox;    // Real numbers         32      +-10^(+-38)
typedef WoodDataBoxImpl<LREAL, DT_LREAL> WoodLRealDataBox; // Long real numbers    64      +-10^(+-308)
// BOOL
typedef WoodDataBoxImpl<BOOL, DT_BOOL> WoodBoolDataBox; // Has two states FALSE, equivalent to 0, and TRUE equivalent to 1.

class WoodData
{
public:
    WoodData(const char *name) : name(name) {}
    virtual ~WoodData() {} // TODO: = 0;

    const String &getName() const { return name; }
    virtual unsigned int getDataType() const = 0;
    virtual WoodDataBox &getDataBox() = 0;

private:
    String name;
};

class WoodOutData : public WoodData
{
public:
    WoodOutData(const char *name) : WoodData(name) {}
    // virtual ~WoodOutData() {}

private:
};

// WoodOutDataImpl<WoodSIntDataBox>;
template <class TDataBox>
class WoodOutDataImpl : public WoodOutData
{
public:
    WoodOutDataImpl(const char *name) : WoodOutData(name), dataBox() {}

    unsigned int getDataType() const { return dataBox.getDataType(); }
    TDataBox &getDataBox() { return dataBox; }

private:
    TDataBox dataBox;
};

// For user extend data type
extern bool extend_check4ConnectDataType(unsigned int outDataType, unsigned int inDataType);

class WoodInData : public WoodData
{
public:
    WoodInData(const char *name) : WoodData(name), outData(nullptr) {}
    // virtual ~WoodInData() {}

    bool isAlreadyConnected() const { return (outData == nullptr) ? false : true; }
    // check that outVariable and inVariable are match
    bool checkForConnectFrom(WoodOutData &outData) const;
    bool connectFrom(WoodOutData &outData)
    {
        if (!checkForConnectFrom(outData))
        {
            return false;
        }
        // connect WoodInData from WoodOutData
        this->outData = &outData;
        return true;
    }
    void disconnect()
    {
        if (this->outData)
        {
            this->outData = nullptr;
        }
    }

    virtual bool sample() = 0; // clone data from 'outData'
    static bool check4ConnectDataType(unsigned int outDataType, unsigned int inDataType)
    {
        switch (outDataType)
        {
        // Integer
        case DT_SINT: // Short integer            8   -128 to þ127
            switch (inDataType)
            {
            case DT_SINT: // Short integer            8   -128 to þ127
                return true;
            case DT_INT: // Integer                  16  -32768 to 32767
                return true;
            case DT_DINT: // Double integer           32  -2^(31) to 2^(31)-1
                return true;
            case DT_LINT: // Long integer             64  -2^(63) to 2^(63)-1
                return true;
            case DT_USINT: // Unsigned short integer   8   0 to 255
                return false;
            case DT_UINT: // Unsigned integer         16  0 to 2^(16)-1
                return false;
            case DT_UDINT: // Unsigned double integer  32  0 to 2^(32)-1
                return false;
            case DT_ULINT: // Unsigned long integer    64  0 to 2^(64)-1
                return false;
            case DT_REAL: // Real numbers         32      +-10^(+-38)
                return true;
            case DT_LREAL: // Long real numbers    64      +-10^(+-308)
                return true;
            case DT_BOOL: // Has two states FALSE, equivalent to 0, and TRUE equivalent to 1.
                return false;
            default:
                break;
            }
            break;
        case DT_INT: // Integer                  16  -32768 to 32767
            switch (inDataType)
            {
            case DT_SINT: // Short integer            8   -128 to þ127
                return false;
            case DT_INT: // Integer                  16  -32768 to 32767
                return true;
            case DT_DINT: // Double integer           32  -2^(31) to 2^(31)-1
                return true;
            case DT_LINT: // Long integer             64  -2^(63) to 2^(63)-1
                return true;
            case DT_USINT: // Unsigned short integer   8   0 to 255
                return false;
            case DT_UINT: // Unsigned integer         16  0 to 2^(16)-1
                return false;
            case DT_UDINT: // Unsigned double integer  32  0 to 2^(32)-1
                return false;
            case DT_ULINT: // Unsigned long integer    64  0 to 2^(64)-1
                return false;
            case DT_REAL: // Real numbers         32      +-10^(+-38)
                return true;
            case DT_LREAL: // Long real numbers    64      +-10^(+-308)
                return true;
            case DT_BOOL: // Has two states FALSE, equivalent to 0, and TRUE equivalent to 1.
                return false;
            default:
                break;
            }
            break;
        case DT_DINT: // Double integer           32  -2^(31) to 2^(31)-1
            switch (inDataType)
            {
            case DT_SINT: // Short integer            8   -128 to þ127
                return false;
            case DT_INT: // Integer                  16  -32768 to 32767
                return false;
            case DT_DINT: // Double integer           32  -2^(31) to 2^(31)-1
                return true;
            case DT_LINT: // Long integer             64  -2^(63) to 2^(63)-1
                return true;
            case DT_USINT: // Unsigned short integer   8   0 to 255
                return false;
            case DT_UINT: // Unsigned integer         16  0 to 2^(16)-1
                return false;
            case DT_UDINT: // Unsigned double integer  32  0 to 2^(32)-1
                return false;
            case DT_ULINT: // Unsigned long integer    64  0 to 2^(64)-1
                return false;
            case DT_REAL: // Real numbers         32      +-10^(+-38)
                return true;
            case DT_LREAL: // Long real numbers    64      +-10^(+-308)
                return true;
            case DT_BOOL: // Has two states FALSE, equivalent to 0, and TRUE equivalent to 1.
                return false;
            default:
                break;
            }
            break;
        case DT_LINT: // Long integer             64  -2^(63) to 2^(63)-1
            switch (inDataType)
            {
            case DT_SINT: // Short integer            8   -128 to þ127
                return false;
            case DT_INT: // Integer                  16  -32768 to 32767
                return false;
            case DT_DINT: // Double integer           32  -2^(31) to 2^(31)-1
                return false;
            case DT_LINT: // Long integer             64  -2^(63) to 2^(63)-1
                return true;
            case DT_USINT: // Unsigned short integer   8   0 to 255
                return false;
            case DT_UINT: // Unsigned integer         16  0 to 2^(16)-1
                return false;
            case DT_UDINT: // Unsigned double integer  32  0 to 2^(32)-1
                return false;
            case DT_ULINT: // Unsigned long integer    64  0 to 2^(64)-1
                return false;
            case DT_REAL: // Real numbers         32      +-10^(+-38)
                return true;
            case DT_LREAL: // Long real numbers    64      +-10^(+-308)
                return true;
            case DT_BOOL: // Has two states FALSE, equivalent to 0, and TRUE equivalent to 1.
                return false;
            default:
                break;
            }
            break;
        case DT_USINT: // Unsigned short integer   8   0 to 255
            switch (inDataType)
            {
            case DT_SINT: // Short integer            8   -128 to þ127
                return false;
            case DT_INT: // Integer                  16  -32768 to 32767
                return true;
            case DT_DINT: // Double integer           32  -2^(31) to 2^(31)-1
                return true;
            case DT_LINT: // Long integer             64  -2^(63) to 2^(63)-1
                return true;
            case DT_USINT: // Unsigned short integer   8   0 to 255
                return true;
            case DT_UINT: // Unsigned integer         16  0 to 2^(16)-1
                return true;
            case DT_UDINT: // Unsigned double integer  32  0 to 2^(32)-1
                return true;
            case DT_ULINT: // Unsigned long integer    64  0 to 2^(64)-1
                return true;
            case DT_REAL: // Real numbers         32      +-10^(+-38)
                return true;
            case DT_LREAL: // Long real numbers    64      +-10^(+-308)
                return true;
            case DT_BOOL: // Has two states FALSE, equivalent to 0, and TRUE equivalent to 1.
                return false;
            default:
                break;
            }
            break;
        case DT_UINT: // Unsigned integer         16  0 to 2^(16)-1
            switch (inDataType)
            {
            case DT_SINT: // Short integer            8   -128 to þ127
                return false;
            case DT_INT: // Integer                  16  -32768 to 32767
                return false;
            case DT_DINT: // Double integer           32  -2^(31) to 2^(31)-1
                return true;
            case DT_LINT: // Long integer             64  -2^(63) to 2^(63)-1
                return true;
            case DT_USINT: // Unsigned short integer   8   0 to 255
                return false;
            case DT_UINT: // Unsigned integer         16  0 to 2^(16)-1
                return true;
            case DT_UDINT: // Unsigned double integer  32  0 to 2^(32)-1
                return true;
            case DT_ULINT: // Unsigned long integer    64  0 to 2^(64)-1
                return true;
            case DT_REAL: // Real numbers         32      +-10^(+-38)
                return true;
            case DT_LREAL: // Long real numbers    64      +-10^(+-308)
                return true;
            case DT_BOOL: // Has two states FALSE, equivalent to 0, and TRUE equivalent to 1.
                return false;
            default:
                break;
            }
            break;
        case DT_UDINT: // Unsigned double integer  32  0 to 2^(32)-1
            switch (inDataType)
            {
            case DT_SINT: // Short integer            8   -128 to þ127
                return false;
            case DT_INT: // Integer                  16  -32768 to 32767
                return false;
            case DT_DINT: // Double integer           32  -2^(31) to 2^(31)-1
                return false;
            case DT_LINT: // Long integer             64  -2^(63) to 2^(63)-1
                return true;
            case DT_USINT: // Unsigned short integer   8   0 to 255
                return false;
            case DT_UINT: // Unsigned integer         16  0 to 2^(16)-1
                return false;
            case DT_UDINT: // Unsigned double integer  32  0 to 2^(32)-1
                return true;
            case DT_ULINT: // Unsigned long integer    64  0 to 2^(64)-1
                return true;
            case DT_REAL: // Real numbers         32      +-10^(+-38)
                return true;
            case DT_LREAL: // Long real numbers    64      +-10^(+-308)
                return true;
            case DT_BOOL: // Has two states FALSE, equivalent to 0, and TRUE equivalent to 1.
                return false;
            default:
                break;
            }
            break;
        case DT_ULINT: // Unsigned long integer    64  0 to 2^(64)-1
            switch (inDataType)
            {
            case DT_SINT: // Short integer            8   -128 to þ127
                return false;
            case DT_INT: // Integer                  16  -32768 to 32767
                return false;
            case DT_DINT: // Double integer           32  -2^(31) to 2^(31)-1
                return false;
            case DT_LINT: // Long integer             64  -2^(63) to 2^(63)-1
                return false;
            case DT_USINT: // Unsigned short integer   8   0 to 255
                return false;
            case DT_UINT: // Unsigned integer         16  0 to 2^(16)-1
                return false;
            case DT_UDINT: // Unsigned double integer  32  0 to 2^(32)-1
                return false;
            case DT_ULINT: // Unsigned long integer    64  0 to 2^(64)-1
                return true;
            case DT_REAL: // Real numbers         32      +-10^(+-38)
                return true;
            case DT_LREAL: // Long real numbers    64      +-10^(+-308)
                return true;
            case DT_BOOL: // Has two states FALSE, equivalent to 0, and TRUE equivalent to 1.
                return false;
            default:
                break;
            }
            break;
        // Floating point (REAL)
        case DT_REAL: // Real numbers         32      +-10^(+-38)
            switch (inDataType)
            {
            case DT_SINT: // Short integer            8   -128 to þ127
                return false;
            case DT_INT: // Integer                  16  -32768 to 32767
                return false;
            case DT_DINT: // Double integer           32  -2^(31) to 2^(31)-1
                return false;
            case DT_LINT: // Long integer             64  -2^(63) to 2^(63)-1
                return false;
            case DT_USINT: // Unsigned short integer   8   0 to 255
                return false;
            case DT_UINT: // Unsigned integer         16  0 to 2^(16)-1
                return false;
            case DT_UDINT: // Unsigned double integer  32  0 to 2^(32)-1
                return false;
            case DT_ULINT: // Unsigned long integer    64  0 to 2^(64)-1
                return false;
            case DT_REAL: // Real numbers         32      +-10^(+-38)
                return true;
            case DT_LREAL: // Long real numbers    64      +-10^(+-308)
                return true;
            case DT_BOOL: // Has two states FALSE, equivalent to 0, and TRUE equivalent to 1.
                return false;
            default:
                break;
            }
            break;
        case DT_LREAL: // Long real numbers    64      +-10^(+-308)
            switch (inDataType)
            {
            case DT_SINT: // Short integer            8   -128 to þ127
                return false;
            case DT_INT: // Integer                  16  -32768 to 32767
                return false;
            case DT_DINT: // Double integer           32  -2^(31) to 2^(31)-1
                return false;
            case DT_LINT: // Long integer             64  -2^(63) to 2^(63)-1
                return false;
            case DT_USINT: // Unsigned short integer   8   0 to 255
                return false;
            case DT_UINT: // Unsigned integer         16  0 to 2^(16)-1
                return false;
            case DT_UDINT: // Unsigned double integer  32  0 to 2^(32)-1
                return false;
            case DT_ULINT: // Unsigned long integer    64  0 to 2^(64)-1
                return false;
            case DT_REAL: // Real numbers         32      +-10^(+-38)
                return false;
            case DT_LREAL: // Long real numbers    64      +-10^(+-308)
                return true;
            case DT_BOOL: // Has two states FALSE, equivalent to 0, and TRUE equivalent to 1.
                return false;
            default:
                break;
            }
            break;
        case DT_BOOL: // Long real numbers    64      +-10^(+-308)
            switch (inDataType)
            {
            case DT_SINT: // Short integer            8   -128 to þ127
                return false;
            case DT_INT: // Integer                  16  -32768 to 32767
                return false;
            case DT_DINT: // Double integer           32  -2^(31) to 2^(31)-1
                return false;
            case DT_LINT: // Long integer             64  -2^(63) to 2^(63)-1
                return false;
            case DT_USINT: // Unsigned short integer   8   0 to 255
                return false;
            case DT_UINT: // Unsigned integer         16  0 to 2^(16)-1
                return false;
            case DT_UDINT: // Unsigned double integer  32  0 to 2^(32)-1
                return false;
            case DT_ULINT: // Unsigned long integer    64  0 to 2^(64)-1
                return false;
            case DT_REAL: // Real numbers         32      +-10^(+-38)
                return false;
            case DT_LREAL: // Long real numbers    64      +-10^(+-308)
                return false;
            case DT_BOOL: // Has two states FALSE, equivalent to 0, and TRUE equivalent to 1.
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
    WoodOutData *getWoodOutData() { return outData; }

private:
    WoodOutData *outData; // start of connection, Output data variable
};

// WoodInDataImpl<WoodSIntDataBox>;
template <class TDataBox>
class WoodInDataImpl : public WoodInData
{
public:
    WoodInDataImpl(const char *name) : WoodInData(name), dataBox() {}

    unsigned int getDataType() const { return dataBox.getDataType(); }
    TDataBox &getDataBox() { return dataBox; }

    // WoodInDataImpl<WoodSIntDataBox>;
    // template <class TDataBox>
    bool sample() // clone data from 'fromData'
    {
        // WoodOutData *outData = getWoodOutData();
        // if (outData)
        // {
        //     dataBox.copy((TDataBox&)outData->getDataBox()); // TODO: error!!!!!!!!!!!!!!!!!!!!!!!!!
        //     return true;
        // }

        // TODO: only same data type fields may be converted!
        WoodOutDataImpl<TDataBox> *outData = (WoodOutDataImpl<TDataBox> *)getWoodOutData();
        if (outData)
        {
            dataBox.copy(outData->getDataBox()); // TODO: error!!!!!!!!!!!!!!!!!!!!!!!!!
            return true;
        }
        return false;
    }

private:
    TDataBox dataBox;
};

// Integer
typedef WoodInDataImpl<WoodSIntDataBox> WoodSIntInData; // Short integer            8   -128 to þ127
// typedef WoodDataBoxImpl<INT, DT_INT> WoodIntDataBox;       // Integer                  16  -32768 to 32767
// typedef WoodDataBoxImpl<DINT, DT_DINT> WoodDIntDataBox;    // Double integer           32  -2^(31) to 2^(31)-1
// typedef WoodDataBoxImpl<LINT, DT_LINT> WoodLIntDataBox;    // Long integer             64  -2^(63) to 2^(63)-1
// typedef WoodDataBoxImpl<USINT, DT_USINT> WoodUSIntDataBox; // Unsigned short integer   8   0 to 255
// typedef WoodDataBoxImpl<UINT, DT_UINT> WoodUIntDataBox;    // Unsigned integer         16  0 to 2^(16)-1
// typedef WoodDataBoxImpl<UDINT, DT_UDINT> WoodUDIntDataBox; // Unsigned double integer  32  0 to 2^(32)-1
// typedef WoodDataBoxImpl<ULINT, DT_ULINT> WoodULIntDataBox; // Unsigned long integer    64  0 to 2^(64)-1
// // Floating point (REAL)
// typedef WoodDataBoxImpl<REAL, DT_REAL> WoodRealDataBox;    // Real numbers         32      +-10^(+-38)
// typedef WoodDataBoxImpl<LREAL, DT_LREAL> WoodLRealDataBox; // Long real numbers    64      +-10^(+-308)
// BOOL
typedef WoodInDataImpl<WoodBoolDataBox> WoodBoolInData; // Has two states FALSE, equivalent to 0, and TRUE equivalent to 1.

#endif // __cplusplus
#endif // WoodData

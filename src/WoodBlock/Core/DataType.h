// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <WString.h>  // Arduino
#include <WoodBlock/Core/DataBox.h>
#include <WoodBlock/Core/Macro.h>
#include <stdint.h>
#include <WoodBlock/Namespace.hpp>

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

// Integer data types
// =============== ========================= ========= ========================
// IEC data type   Description               Bits      Range
// =============== ========================= ========= ========================
// SINT            Short integer             8         -128 to 127
// INT           Integer                   16        -32768 to 32767
// DINT          Double integer            32        -2^31 to 2^31-1
// LINT          Long integer              64        -2^63 to 2^63-1
// USINT         Unsigned short integer    8         0 to 255
// UINT          Unsigned integer          16        0 to 2^16-1
// UDINT         Unsigned double integer   32        0 to 2^32-1
// ULINT         Unsigned long integer     64        0 to 2^64-1
// =============== ========================= ========= ========================

// Floating point (Real) data types
// =============== ========================= ========= ========================
// IEC data type   Description               Bits      Range
// =============== ========================= ========= ========================
// Real            Real numbers              32        +-10^(+-38)    (Note 1)
// LReal           Long real numbers         64        +-10^(+-308)   (Note 2)
// =============== ========================= ========= ========================
// Note 1: Real values have a precision of 1 part in 2^23.
// Note 2: LReal values have a precision of 1 part in 2^52.

// Boolean data type
// =============== ==========================================================
// IEC data type   Description
// =============== ==========================================================
// Bool            Has two states. FALSE equivalent to 0,
//                 and TRUE equivalent to 1.
// =============== ==========================================================

typedef int8_t SINT;     // Short integer            8   -128 to 127
typedef int16_t INT;     // Integer                  16  -32768 to 32767
typedef int32_t DINT;    // Double integer           32  -2^31 to 2^31-1
typedef int64_t LINT;    // Long integer             64  -2^63 to 2^63-1
typedef uint8_t USINT;   // Unsigned short integer   8   0 to 255
typedef uint16_t UINT;   // Unsigned integer         16  0 to 2^16-1
typedef uint32_t UDINT;  // Unsigned double integer  32  0 to 2^32-1
typedef uint64_t ULINT;  // Unsigned long integer    64  0 to 2^64-1
typedef float REAL;      // Real numbers         32      +-10^(+-38)
typedef double LREAL;    // Long real numbers    64      +-10^(+-308)
typedef bool BOOL;       // Has two states. FALSE = 0, TRUE = 1.

extern const char SINT_NAME[];
extern const char INT_NAME[];
extern const char DINT_NAME[];
extern const char LINT_NAME[];
extern const char USINT_NAME[];
extern const char UINT_NAME[];
extern const char UDINT_NAME[];
extern const char ULINT_NAME[];
extern const char REAL_NAME[];
extern const char LREAL_NAME[];
extern const char BOOL_NAME[];

typedef DataBoxImpl<SINT, SINT_NAME> SInt;
typedef DataBoxImpl<INT, INT_NAME> Int;
typedef DataBoxImpl<DINT, DINT_NAME> DInt;
typedef DataBoxImpl<LINT, LINT_NAME> LInt;
typedef DataBoxImpl<USINT, USINT_NAME> USInt;
typedef DataBoxImpl<UINT, UINT_NAME> UInt;
typedef DataBoxImpl<UDINT, UDINT_NAME> UDInt;
typedef DataBoxImpl<ULINT, ULINT_NAME> ULInt;
typedef DataBoxImpl<REAL, REAL_NAME> Real;
typedef DataBoxImpl<LREAL, LREAL_NAME> LReal;
typedef DataBoxImpl<BOOL, BOOL_NAME> Bool;

WOODBLOCK_END_PUBLIC_NAMESPACE
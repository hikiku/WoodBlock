// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <WString.h>  // Arduino

#include <WoodBlock/Namespace.hpp>
// #include <WoodBlock/Macro.h>

// #include <WoodBlock/DataTypes/DataTypes.h>
#include <WoodBlock/DataTypes/ElementaryDataTypes.h>
// #include <WoodBlock/DataTypes/GenericDataTypes.h>
#include <WoodBlock/DataTypes/DataBox.h>
#include <WoodBlock/DataTypes/DataBoxType.h>


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
extern const char SINT_NAME[];
extern const char INT_NAME[];
extern const char DINT_NAME[];
extern const char LINT_NAME[];
extern const char USINT_NAME[];
extern const char UINT_NAME[];
extern const char UDINT_NAME[];
extern const char ULINT_NAME[];

typedef DataBoxImpl<SINT, SINT_NAME> SInt;
typedef DataBoxImpl<INT, INT_NAME> Int;
typedef DataBoxImpl<DINT, DINT_NAME> DInt;
typedef DataBoxImpl<LINT, LINT_NAME> LInt;
typedef DataBoxImpl<USINT, USINT_NAME> USInt;
typedef DataBoxImpl<UINT, UINT_NAME> UInt;
typedef DataBoxImpl<UDINT, UDINT_NAME> UDInt;
typedef DataBoxImpl<ULINT, ULINT_NAME> ULInt;

// Floating point (Real) data types
// =============== ========================= ========= ========================
// IEC data type   Description               Bits      Range
// =============== ========================= ========= ========================
// Real            Real numbers              32        +-10^(+-38)    (Note 1)
// LReal           Long real numbers         64        +-10^(+-308)   (Note 2)
// =============== ========================= ========= ========================
// Note 1: Real values have a precision of 1 part in 2^23.
// Note 2: LReal values have a precision of 1 part in 2^52.
extern const char REAL_NAME[];
extern const char LREAL_NAME[];
typedef DataBoxImpl<REAL, REAL_NAME> Real;
typedef DataBoxImpl<LREAL, LREAL_NAME> LReal;

// // Boolean data type
// // =============== ==========================================================
// // IEC data type   Description
// // =============== ==========================================================
// // Bool            Has two states. FALSE equivalent to 0,
// //                 and TRUE equivalent to 1.
// // =============== ==========================================================
extern const char BOOL_NAME[];
typedef DataBoxImpl<BOOL, BOOL_NAME> Bool;

// Time data types
// ========= ============= ===== =============================================
// Data type Description   Bits  Usage
// ========= ============= ===== =============================================
// TIME      Time duration Note  Storing the duration of time after an event
// ========= ============= ===== =============================================
// Note: The length and precision of this data type are implementation
// dependent. The TIME data type is used to store durations, i.e. in days,
// hours, minutes, seconds and milliseconds
extern const char TIME_NAME[];
typedef DataBoxImpl<TIME, TIME_NAME> Time;

// Dates and times of day data types
// ==================== ==================== ====== ===========================
// Data type            Description          Bits   Usage
// ==================== ==================== ====== ===========================
// DATE                 Calendar Date        Note   Storing calendar dates
// TIME_OF_DAY or TOD   Time of day          Note   Storing times of the day,
//                                                  i.e. real time clock
// DATE_AND_TIME or DT  Date and time of day Note   Storing the date and the
//                                                  time of day
// ==================== ==================== ====== ===========================
// Note: The length of this data type is implementation dependent.

extern const char DATE_NAME[];
typedef DataBoxImpl<DATE, DATE_NAME> Date;

extern const char TOD_NAME[];
typedef DataBoxImpl<TOD, TOD_NAME> ToD;

extern const char DT_NAME[];
typedef DataBoxImpl<DT, DT_NAME> Dt;

// String data type
// ========== ===================== =============== ===========================
// Data type  Description           Bits            Usage
// ========== ===================== =============== ===========================
// STRING     Character strings     8 per character Storing textual information
// ========== ===================== =============== ===========================
extern const char STRING_NAME[];
typedef DataBoxImpl<STRING, STRING_NAME> XString;

// Double-byte character string data type
// ========== ===================== ================ ==========================
// Data type  Description           Bits             Usage
// ========== ===================== ================ ==========================
// WSTRING    Character strings     16 per character Storing unicode textual
//                                                   information
// ========== ===================== ================ ==========================
extern const char WSTRING_NAME[];
typedef DataBoxImpl<WSTRING, WSTRING_NAME> XWString;

// Bit string data types
// ========== ===================== ======= ===================================
// Data type  Description           Bits    Usage
// ========== ===================== ======= ===================================
// BOOL       Bit string of 1 bit   1       Digital, logical states
// BYTE       Bit string of 8 bits  8       Binary information
// WORD       Bit string of 16 bits 16      ’’ ’’
// DWORD      Bit string of 32 bits 32      ’’ ’’
// LWORD      Bit string of 64 bits 64      ’’ ’’
// ========== ===================== ======= ===================================
extern const char BYTE_NAME[];
extern const char WORD_NAME[];
extern const char DWORD_NAME[];
extern const char LWORD_NAME[];
typedef DataBoxImpl<BYTE, BYTE_NAME> Byte;
typedef DataBoxImpl<WORD, WORD_NAME> Word;
typedef DataBoxImpl<DWORD, DWORD_NAME> DWord;
typedef DataBoxImpl<LWORD, LWORD_NAME> LWord;

WOODBLOCK_END_PUBLIC_NAMESPACE
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
typedef int8_t SINT;     // Short integer            8   -128 to 127
typedef int16_t INT;     // Integer                  16  -32768 to 32767
typedef int32_t DINT;    // Double integer           32  -2^31 to 2^31-1
typedef int64_t LINT;    // Long integer             64  -2^63 to 2^63-1
typedef uint8_t USINT;   // Unsigned short integer   8   0 to 255
typedef uint16_t UINT;   // Unsigned integer         16  0 to 2^16-1
typedef uint32_t UDINT;  // Unsigned double integer  32  0 to 2^32-1
typedef uint64_t ULINT;  // Unsigned long integer    64  0 to 2^64-1

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
typedef float REAL;    // Real numbers         32      +-10^(+-38)
typedef double LREAL;  // Long real numbers    64      +-10^(+-308)
extern const char REAL_NAME[];
extern const char LREAL_NAME[];
typedef DataBoxImpl<REAL, REAL_NAME> Real;
typedef DataBoxImpl<LREAL, LREAL_NAME> LReal;

// Boolean data type
// =============== ==========================================================
// IEC data type   Description
// =============== ==========================================================
// Bool            Has two states. FALSE equivalent to 0,
//                 and TRUE equivalent to 1.
// =============== ==========================================================
typedef bool BOOL;  // Has two states. FALSE = 0, TRUE = 1.
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
typedef struct tagTIME {
  int32_t days;
  int16_t hours;         // 0~23
  int16_t minutes;       // 0~59
  int16_t seconds;       // 0~59, to 61
  int16_t milliseconds;  // 0~999
} TIME;
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

// #include <ctime>
// struct tm {
//    int tm_sec;   // seconds of minutes from 0 to 61
//    int tm_min;   // minutes of hour from 0 to 59
//    int tm_hour;  // hours of day from 0 to 24
//    int tm_mday;  // day of month from 1 to 31
//    int tm_mon;   // month of year from 0 to 11
//    int tm_year;  // year since 1900
//    int tm_wday;  // days since sunday
//    int tm_yday;  // days since January 1st
//    int tm_isdst; // hours of daylight savings time
// }

typedef struct tmDate {
  int tm_mday;   // day of month from 1 to 31
  int tm_mon;    // month of year from 0 to 11
  int tm_year;   // year since 1900
  int tm_wday;   // days since sunday
  int tm_yday;   // days since January 1st
  int tm_isdst;  // hours of daylight savings time
} DATE;
extern const char DATE_NAME[];
typedef DataBoxImpl<DATE, DATE_NAME> Date;

typedef struct tmTime {
  int tm_sec;   // seconds of minutes from 0 to 61
  int tm_min;   // minutes of hour from 0 to 59
  int tm_hour;  // hours of day from 0 to 24
} TIME_OF_DAY, TOD;
extern const char TOD_NAME[];
typedef DataBoxImpl<TOD, TOD_NAME> ToD;

typedef struct tm DATE_AND_TIME, DT;
extern const char DT_NAME[];
typedef DataBoxImpl<DT, DT_NAME> Dt;

// String data type
// ========== ===================== =============== ===========================
// Data type  Description           Bits            Usage
// ========== ===================== =============== ===========================
// STRING     Character strings     8 per character Storing textual information
// ========== ===================== =============== ===========================
typedef String STRING;
extern const char STRING_NAME[];
typedef DataBoxImpl<STRING, STRING_NAME> XString;

// Double-byte character string data type
// ========== ===================== ================ ==========================
// Data type  Description           Bits             Usage
// ========== ===================== ================ ==========================
// WSTRING    Character strings     16 per character Storing unicode textual
//                                                   information
// ========== ===================== ================ ==========================
typedef String WSTRING;
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
typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint64_t LWORD;
extern const char BYTE_NAME[];
extern const char WORD_NAME[];
extern const char DWORD_NAME[];
extern const char LWORD_NAME[];
typedef DataBoxImpl<BYTE, BYTE_NAME> Byte;
typedef DataBoxImpl<WORD, WORD_NAME> Word;
typedef DataBoxImpl<DWORD, DWORD_NAME> DWord;
typedef DataBoxImpl<LWORD, LWORD_NAME> LWord;

WOODBLOCK_END_PUBLIC_NAMESPACE
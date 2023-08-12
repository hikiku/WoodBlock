// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <WString.h>  // Arduino
#include <stdbool.h>
#include <stdint.h>
#include <WoodBlock/Namespace.hpp>

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

/* Boolean data type
=============== ==========================================================
IEC data type   Description
=============== ==========================================================
Bool            Has two states. FALSE equivalent to 0,
                and TRUE equivalent to 1.
=============== ==========================================================
*/
typedef bool BOOL;  // Has two states. FALSE = 0, TRUE = 1.

#ifndef FALSE
#  define FALSE 0
#endif
#ifndef TRUE
#  define TRUE 1
#endif

/* Integer data types
=============== ========================= ========= ========================
IEC data type   Description               Bits      Range
=============== ========================= ========= ========================
SINT            Short integer             8         -128 to 127
INT             Integer                   16        -32768 to 32767
DINT            Double integer            32        -2^31 to 2^31-1
LINT            Long integer              64        -2^63 to 2^63-1
USINT           Unsigned short integer    8         0 to 255
UINT            Unsigned integer          16        0 to 2^16-1
UDINT           Unsigned double integer   32        0 to 2^32-1
ULINT           Unsigned long integer     64        0 to 2^64-1
=============== ========================= ========= ========================
*/
typedef int8_t SINT;     // Short integer            8   -128 to 127
typedef int16_t INT;     // Integer                  16  -32768 to 32767
typedef int32_t DINT;    // Double integer           32  -2^31 to 2^31-1
typedef int64_t LINT;    // Long integer             64  -2^63 to 2^63-1
typedef uint8_t USINT;   // Unsigned short integer   8   0 to 255
typedef uint16_t UINT;   // Unsigned integer         16  0 to 2^16-1
typedef uint32_t UDINT;  // Unsigned double integer  32  0 to 2^32-1
typedef uint64_t ULINT;  // Unsigned long integer    64  0 to 2^64-1

/* Floating point (Real) data types
=============== ========================= ========= ========================
IEC data type   Description               Bits      Range
=============== ========================= ========= ========================
Real            Real numbers              32        +-10^(+-38)    (Note 1)
LReal           Long real numbers         64        +-10^(+-308)   (Note 2)
=============== ========================= ========= ========================
Note 1: Real values have a precision of 1 part in 2^23.
Note 2: LReal values have a precision of 1 part in 2^52.
*/
typedef float REAL;    // Real numbers         32      +-10^(+-38)
typedef double LREAL;  // Long real numbers    64      +-10^(+-308)

/* Duration/Time data types
========= ============= ===== =============================================
Data type Description   Bits  Usage
========= ============= ===== =============================================
TIME      Time duration Note  b Storing the duration of time after an event
LTIME                   64    m, q
========= ============= ===== =============================================
Note: The length and precision of this data type are implementation
    dependent. The TIME data type is used to store durations, i.e. in days,
    hours, minutes, seconds and milliseconds
b    The range of values and precision of representation in these data types
     is Implementer specific.
m    The data type LTIME is a signed 64-bit integer with unit of nanoseconds.
q    The update accuracy of the values of this time format is Implementer
     specific, i.e. the value is given in nanoseconds, but it may be updated
     every microsecond or millisecond.
*/
typedef struct tagTIME {
  int32_t days;
  int16_t hours;         // 0~23
  int16_t minutes;       // 0~59
  int16_t seconds;       // 0~59, to 61
  int16_t milliseconds;  // 0~999
} TIME;                  // TODO: ...........................................
typedef int64_t LTIME;

/* Dates and times of day data types
===================== ==================== ====== =========================
Data type             Description          Bits   Usage
===================== ==================== ====== =========================
DATE                  Date (only)          Note   b Storing calendar dates
LDATE                 Long Date (only)     64     n
TIME_OF_DAY or TOD    Time of day (only)   Note   b Storing times of the day,
                                                    i.e. real time clock.
LTIME_OF_DAY or LTOD  Time of day (only)   64     o, q
DATE_AND_TIME or DT   Date and time of day Note   b Storing the date and the
                                                    time of day
LDATE_AND_TIME or LDT Date and time of Day 64     p, q
===================== ==================== ====== ===========================
Note: The length of this data type is implementation dependent.
b   The range of values and precision of representation in these data types is
    Implementer specific.
n   The data type LDATE is a signed 64-bit integer with unit of nanoseconds
    with starting date 1970-01-01.
o   The data type LDT is a signed 64-bit integer with unit of nanoseconds
    with starting date 1970-01-01-00:00:00.
p   The data type LTOD is a signed 64-bit integer with unit of nanoseconds
    with starting time midnight with TOD#00:00:00.
q   The update accuracy of the values of this time format is Implementer
    specific, i.e. the value is given in nanoseconds, but it may be updated
    every microsecond or millisecond.
*/

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
} DATE;          // TODO: ...................................................
typedef int64_t LDATE;

typedef struct tmTime {
  int tm_sec;        // seconds of minutes from 0 to 61
  int tm_min;        // minutes of hour from 0 to 59
  int tm_hour;       // hours of day from 0 to 24
} TIME_OF_DAY, TOD;  // TODO: ...............................................
typedef int64_t LTIME_OF_DAY, LTOD;

typedef struct tm DATE_AND_TIME, DT;  // TODO: ..............................
typedef int64_t LDATE_AND_TIME, LDT;

/* String data type - Variable-length character string
========= ================= ================ ===========================
Data type Description       Bits             Usage
========= ================= ================ ===========================
STRING    Character strings 8 per character  i, g, k, l
                                             Storing textual information
WSTRING   Character strings 16 per character i, g, k, l
                                             Storing unicode textual
                                             information
========= ================= ================ ===========================
i   The value of N indicates the number of bits/character for this data type.
g   A numeric range of values does not apply to this data type
k   The maximum allowed length of STRING and WSTRING variables is Implementer
    specific.
l   The character encoding used for CHAR, STRING, WCHAR, and WSTRING is ISO/IEC
    10646 (see 6.3.3).
*/
typedef String STRING;   // TODO: ........................................
typedef String WSTRING;  // TODO: ........................................

/* character
========= ===================== ================ ===============
Data type Description           Bits             Usage
========= ===================== ================ ===============
CHAR      Single-byte character 8 per character  g, l
WCHAR     Double-byte character 16 per character g, l
========= ===================== ================ ===============
g   A numeric range of values does not apply to this data type
l   The character encoding used for CHAR, STRING, WCHAR, and WSTRING is ISO/IEC
    10646 (see 6.3.3).
*/
typedef char CHAR;      // TODO: ..........................................
typedef wchar_t WCHAR;  // TODO: ............................................

/* Bit string data types
========= ===================== ==== ============================
Data type Description           Bits Usage
========= ===================== ==== ============================
BYTE      Bit string of 8 bits  8    j, g Digital, logical states
WORD      Bit string of 16 bits 16   j, g Binary information
DWORD     Bit string of 32 bits 32   j, g ’’ ’’
LWORD     Bit string of 64 bits 64   j, g ’’ ’’
========= ===================== ==== ============================
j   The value of N indicates the number of bits in the bit string
    for this data type.
g   A numeric range of values does not apply to this data type
*/
typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint64_t LWORD;

WOODBLOCK_END_PUBLIC_NAMESPACE

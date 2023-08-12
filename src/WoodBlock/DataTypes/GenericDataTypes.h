// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

// #include <WString.h>  // Arduino
// #include <stdbool.h>
// #include <stdint.h>
#include <WoodBlock/Namespace.hpp>

// TODO: ...............................................................

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

template <class T>
class ANY {};

template <class T>
class ANY_DERIVED : public ANY<T> {};

template <class T>
class ANY_ELEMENTARY : public ANY<T> {};

template <class T>
class ANY_MAGNITUDE : public ANY_ELEMENTARY<T> {};

template <class T>
class ANY_NUM : public ANY_MAGNITUDE<T> {};

template <class T>
class ANY_REAL : public ANY_NUM<T> {};

template <class T>
class ANY_INT : public ANY_NUM<T> {};

template <class T>
class ANY_UNSIGNED : public ANY_INT<T> {};

template <class T>
class ANY_SIGNED : public ANY_INT<T> {};

template <class T>
class ANY_DURATION : public ANY_MAGNITUDE<T> {};

template <class T>
class ANY_BIT : public ANY_ELEMENTARY<T> {};

template <class T>
class ANY_CHARS : public ANY_ELEMENTARY<T> {};

template <class T>
class ANY_STRING : public ANY_CHARS<T> {};

template <class T>
class ANY_CHAR : public ANY_CHARS<T> {};

template <class T>
class ANY_DATE : public ANY_ELEMENTARY<T> {};

// template <class T> using xxx = ANY_REAL<T>;
typedef ANY_REAL<REAL> REAL_T;
typedef ANY_REAL<LREAL> LREAL_T;

typedef ANY_UNSIGNED<USINT> USINT_T;
typedef ANY_UNSIGNED<UINT> UINT_T;
typedef ANY_UNSIGNED<UDINT> UDINT_T;
typedef ANY_UNSIGNED<ULINT> ULINT_T;

typedef ANY_SIGNED<SINT> SINT_T;
typedef ANY_SIGNED<INT> INT_T;
typedef ANY_SIGNED<DINT> DINT_T;
typedef ANY_SIGNED<LINT> LINT_T;

typedef ANY_DURATION<TIME> TIME_T;
typedef ANY_DURATION<LTIME> LTIME_T;

typedef ANY_BIT<BOOL> BOOL_T;
typedef ANY_BIT<BYTE> BYTE_T;
typedef ANY_BIT<WORD> WORD_T;
typedef ANY_BIT<DWORD> DWORD_T;
typedef ANY_BIT<LWORD> LWORD_T;

typedef ANY_STRING<STRING> STRING_T;
typedef ANY_STRING<WSTRING> WSTRING_T;

typedef ANY_CHAR<CHAR> CHAR_T;
typedef ANY_CHAR<WCHAR> WCHAR_T;

typedef ANY_DATE<DATE_AND_TIME> DATE_AND_TIME_T, DT_T;
typedef ANY_DATE<LDT> LDT_T;
typedef ANY_DATE<DATE> DATE_T;
typedef ANY_DATE<TIME_OF_DAY> TIME_OF_DAY_T, TOD_T;
typedef ANY_DATE<LTOD> LTOD_T;

WOODBLOCK_END_PUBLIC_NAMESPACE

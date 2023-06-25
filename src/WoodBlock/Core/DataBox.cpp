// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#include <WoodBlock/Core/DataBox.h>

WOODBLOCK_BEGIN_PUBLIC_NAMESPACE

bool DataBox::check4ConnectDataType(unsigned int outNumberOfDataType, unsigned int inNumberOfDataType) {
//   unsigned int outNumberOfDataType = outBox.getNumberOfDataType();
//   unsigned int inNumberOfDataType = inBox.getNumberOfDataType();

  // TODO: check to converting data type

  if (outNumberOfDataType == inNumberOfDataType) {
    return true;
  } else {
    // // TODO: printf(WARNING, "outNumberOfDataType(%u) and inNumberOfDataType(%u) are
    // // not matched!\n", outNumberOfDataType, inNumberOfDataType)
    // return true;
  }

//   switch (outNumberOfDataType) {
//     // Integer
//     case SINT_NO:  // Short integer            8   -128 to þ127
//       switch (inNumberOfDataType) {
//         case SINT_NO:  // Short integer            8   -128 to þ127
//           return true;
//         case INT_NO:  // Integer                  16  -32768 to 32767
//           return true;
//         case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
//           return true;
//         case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
//           return true;
//         case USINT_NO:  // Unsigned short integer   8   0 to 255
//           return false;
//         case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
//           return false;
//         case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
//           return false;
//         case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
//           return false;
//         case REAL_NO:  // Real numbers         32      +-10^(+-38)
//           return true;
//         case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
//           return true;
//         case BOOL_NO:  // Has two states FALSE, equivalent to 0, and TRUE
//                        // equivalent to 1.
//           return false;
//         default:
//           break;
//       }
//       break;
//     case INT_NO:  // Integer                  16  -32768 to 32767
//       switch (inNumberOfDataType) {
//         case SINT_NO:  // Short integer            8   -128 to þ127
//           return false;
//         case INT_NO:  // Integer                  16  -32768 to 32767
//           return true;
//         case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
//           return true;
//         case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
//           return true;
//         case USINT_NO:  // Unsigned short integer   8   0 to 255
//           return false;
//         case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
//           return false;
//         case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
//           return false;
//         case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
//           return false;
//         case REAL_NO:  // Real numbers         32      +-10^(+-38)
//           return true;
//         case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
//           return true;
//         case BOOL_NO:  // Has two states FALSE, equivalent to 0, and TRUE
//                        // equivalent to 1.
//           return false;
//         default:
//           break;
//       }
//       break;
//     case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
//       switch (inNumberOfDataType) {
//         case SINT_NO:  // Short integer            8   -128 to þ127
//           return false;
//         case INT_NO:  // Integer                  16  -32768 to 32767
//           return false;
//         case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
//           return true;
//         case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
//           return true;
//         case USINT_NO:  // Unsigned short integer   8   0 to 255
//           return false;
//         case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
//           return false;
//         case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
//           return false;
//         case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
//           return false;
//         case REAL_NO:  // Real numbers         32      +-10^(+-38)
//           return true;
//         case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
//           return true;
//         case BOOL_NO:  // Has two states FALSE, equivalent to 0, and TRUE
//                        // equivalent to 1.
//           return false;
//         default:
//           break;
//       }
//       break;
//     case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
//       switch (inNumberOfDataType) {
//         case SINT_NO:  // Short integer            8   -128 to þ127
//           return false;
//         case INT_NO:  // Integer                  16  -32768 to 32767
//           return false;
//         case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
//           return false;
//         case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
//           return true;
//         case USINT_NO:  // Unsigned short integer   8   0 to 255
//           return false;
//         case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
//           return false;
//         case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
//           return false;
//         case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
//           return false;
//         case REAL_NO:  // Real numbers         32      +-10^(+-38)
//           return true;
//         case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
//           return true;
//         case BOOL_NO:  // Has two states FALSE, equivalent to 0, and TRUE
//                        // equivalent to 1.
//           return false;
//         default:
//           break;
//       }
//       break;
//     case USINT_NO:  // Unsigned short integer   8   0 to 255
//       switch (inNumberOfDataType) {
//         case SINT_NO:  // Short integer            8   -128 to þ127
//           return false;
//         case INT_NO:  // Integer                  16  -32768 to 32767
//           return true;
//         case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
//           return true;
//         case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
//           return true;
//         case USINT_NO:  // Unsigned short integer   8   0 to 255
//           return true;
//         case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
//           return true;
//         case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
//           return true;
//         case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
//           return true;
//         case REAL_NO:  // Real numbers         32      +-10^(+-38)
//           return true;
//         case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
//           return true;
//         case BOOL_NO:  // Has two states FALSE, equivalent to 0, and TRUE
//                        // equivalent to 1.
//           return false;
//         default:
//           break;
//       }
//       break;
//     case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
//       switch (inNumberOfDataType) {
//         case SINT_NO:  // Short integer            8   -128 to þ127
//           return false;
//         case INT_NO:  // Integer                  16  -32768 to 32767
//           return false;
//         case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
//           return true;
//         case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
//           return true;
//         case USINT_NO:  // Unsigned short integer   8   0 to 255
//           return false;
//         case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
//           return true;
//         case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
//           return true;
//         case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
//           return true;
//         case REAL_NO:  // Real numbers         32      +-10^(+-38)
//           return true;
//         case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
//           return true;
//         case BOOL_NO:  // Has two states FALSE, equivalent to 0, and TRUE
//                        // equivalent to 1.
//           return false;
//         default:
//           break;
//       }
//       break;
//     case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
//       switch (inNumberOfDataType) {
//         case SINT_NO:  // Short integer            8   -128 to þ127
//           return false;
//         case INT_NO:  // Integer                  16  -32768 to 32767
//           return false;
//         case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
//           return false;
//         case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
//           return true;
//         case USINT_NO:  // Unsigned short integer   8   0 to 255
//           return false;
//         case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
//           return false;
//         case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
//           return true;
//         case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
//           return true;
//         case REAL_NO:  // Real numbers         32      +-10^(+-38)
//           return true;
//         case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
//           return true;
//         case BOOL_NO:  // Has two states FALSE, equivalent to 0, and TRUE
//                        // equivalent to 1.
//           return false;
//         default:
//           break;
//       }
//       break;
//     case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
//       switch (inNumberOfDataType) {
//         case SINT_NO:  // Short integer            8   -128 to þ127
//           return false;
//         case INT_NO:  // Integer                  16  -32768 to 32767
//           return false;
//         case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
//           return false;
//         case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
//           return false;
//         case USINT_NO:  // Unsigned short integer   8   0 to 255
//           return false;
//         case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
//           return false;
//         case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
//           return false;
//         case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
//           return true;
//         case REAL_NO:  // Real numbers         32      +-10^(+-38)
//           return true;
//         case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
//           return true;
//         case BOOL_NO:  // Has two states FALSE, equivalent to 0, and TRUE
//                        // equivalent to 1.
//           return false;
//         default:
//           break;
//       }
//       break;
//     // Floating point (Real)
//     case REAL_NO:  // Real numbers         32      +-10^(+-38)
//       switch (inNumberOfDataType) {
//         case SINT_NO:  // Short integer            8   -128 to þ127
//           return false;
//         case INT_NO:  // Integer                  16  -32768 to 32767
//           return false;
//         case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
//           return false;
//         case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
//           return false;
//         case USINT_NO:  // Unsigned short integer   8   0 to 255
//           return false;
//         case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
//           return false;
//         case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
//           return false;
//         case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
//           return false;
//         case REAL_NO:  // Real numbers         32      +-10^(+-38)
//           return true;
//         case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
//           return true;
//         case BOOL_NO:  // Has two states FALSE, equivalent to 0, and TRUE
//                        // equivalent to 1.
//           return false;
//         default:
//           break;
//       }
//       break;
//     case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
//       switch (inNumberOfDataType) {
//         case SINT_NO:  // Short integer            8   -128 to þ127
//           return false;
//         case INT_NO:  // Integer                  16  -32768 to 32767
//           return false;
//         case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
//           return false;
//         case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
//           return false;
//         case USINT_NO:  // Unsigned short integer   8   0 to 255
//           return false;
//         case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
//           return false;
//         case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
//           return false;
//         case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
//           return false;
//         case REAL_NO:  // Real numbers         32      +-10^(+-38)
//           return false;
//         case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
//           return true;
//         case BOOL_NO:  // Has two states FALSE, equivalent to 0, and TRUE
//                        // equivalent to 1.
//           return false;
//         default:
//           break;
//       }
//       break;
//     case BOOL_NO:  // Long real numbers    64      +-10^(+-308)
//       switch (inNumberOfDataType) {
//         case SINT_NO:  // Short integer            8   -128 to þ127
//           return false;
//         case INT_NO:  // Integer                  16  -32768 to 32767
//           return false;
//         case DINT_NO:  // Double integer           32  -2^(31) to 2^(31)-1
//           return false;
//         case LINT_NO:  // Long integer             64  -2^(63) to 2^(63)-1
//           return false;
//         case USINT_NO:  // Unsigned short integer   8   0 to 255
//           return false;
//         case UINT_NO:  // Unsigned integer         16  0 to 2^(16)-1
//           return false;
//         case UDINT_NO:  // Unsigned double integer  32  0 to 2^(32)-1
//           return false;
//         case ULINT_NO:  // Unsigned long integer    64  0 to 2^(64)-1
//           return false;
//         case REAL_NO:  // Real numbers         32      +-10^(+-38)
//           return false;
//         case LREAL_NO:  // Long real numbers    64      +-10^(+-308)
//           return false;
//         case BOOL_NO:  // Has two states FALSE, equivalent to 0, and TRUE
//                        // equivalent to 1.
//           return true;
//         default:
//           break;
//       }
//       break;
//     default:
//       break;
//   }

  return extend_check4ConnectDataType(outNumberOfDataType, inNumberOfDataType);
}

WOODBLOCK_END_PUBLIC_NAMESPACE
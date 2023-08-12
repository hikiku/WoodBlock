// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#ifndef NULL
#  define NULL (void*)0
#endif

// TOOD: print(ERROR, "%s is NULL!\n", #a);
#define WB_CHECK_PTR(a)                    \
  if (((a) == nullptr) || ((a) == NULL)) { \
    return;                                \
  }

// TOOD: print(ERROR, "%s is NULL!\n", #a);
#define WB_CHECK_PTR_RETURN_VALUE(a, result) \
  if (((a) == nullptr) || ((a) == NULL)) {   \
    return (result);                         \
  }

// TOOD: print(ERROR, "%s is NULL!\n", #a);
#define WB_CHECK_EXP(a) \
  if (a) {              \
    return;             \
  }

// TOOD: print(ERROR, "%s is NULL!\n", #a);
#define WB_CHECK_EXP_RETURN_VALUE(a, result) \
  if (a) {                                   \
    return (result);                         \
  }

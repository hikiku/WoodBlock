// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#define WB_OUT(format, ...) Serial.printf(format, ##__VA_ARGS__)

#define WB_LOGE(format, ...)                                        \
  Serial.printf("[WB][E]" format " %s()@%s,%d \r\n", ##__VA_ARGS__, \
                __FUNCTION__, __FILE__, __LINE__)  //"[WB][E] "
#define WB_LOGW(format, ...)                                        \
  Serial.printf("[WB][W]" format " %s()@%s,%d \r\n", ##__VA_ARGS__, \
                __FUNCTION__, __FILE__, __LINE__)  //"[WB][W] "
#define WB_LOGI(format, ...)                                        \
  Serial.printf("[WB][I]" format " %s()@%s,%d \r\n", ##__VA_ARGS__, \
                __FUNCTION__, __FILE__, __LINE__)  //"[WB][I] "
#define WB_LOGD(format, ...)                                        \
  Serial.printf("[WB][D]" format " %s()@%s,%d \r\n", ##__VA_ARGS__, \
                __FUNCTION__, __FILE__, __LINE__)  //"[WB][D] "
#define WB_LOGV(format, ...)                                        \
  Serial.printf("[WB][V]" format " %s()@%s,%d \r\n", ##__VA_ARGS__, \
                __FUNCTION__, __FILE__, __LINE__)  //"[WB][V] "

#ifndef NULL
#  define NULL (void*)0
#endif

// TOOD: print(ERROR, "%s is NULL!\n", #a);
#define WB_CHECK_PTR(a)                    \
  if (((a) == nullptr) || ((a) == NULL)) { \
    WB_LOGE(#a " is NULL!");               \
    return;                                \
  }

// TOOD: print(ERROR, "%s is NULL!\n", #a);
#define WB_CHECK_PTR_RETURN_VALUE(a, result) \
  if (((a) == nullptr) || ((a) == NULL)) {   \
    WB_LOGE(#a " is NULL!");                 \
    return (result);                         \
  }

// TOOD: print(ERROR, "%s is NULL!\n", #a);
#define WB_CHECK_EXP(a)     \
  if (a) {                  \
    WB_LOGE(#a "is true!"); \
    return;                 \
  }

// TOOD: print(ERROR, "%s is NULL!\n", #a);
#define WB_CHECK_EXP_RETURN_VALUE(a, result) \
  if (a) {                                   \
    WB_LOGE(#a "is true!");                  \
    return (result);                         \
  }

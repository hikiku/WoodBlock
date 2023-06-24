// WoodBlock - https://hikiku.io
// Copyright © 2023, HiKiku
// MIT License

#pragma once

#include <WoodBlock/version.hpp>

#define WOODBLOCK_BEGIN_PUBLIC_NAMESPACE \
  namespace WoodBlock {

#define WOODBLOCK_END_PUBLIC_NAMESPACE \
  }

#define WOODBLOCK_BEGIN_PRIVATE_NAMESPACE        \
  namespace WoodBlock {                          \
  namespace detail {

#define WOODBLOCK_END_PRIVATE_NAMESPACE \
  }                                       \
  }

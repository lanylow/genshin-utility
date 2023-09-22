#pragma once

#include <cstdio>

namespace utils::console {
  [[maybe_unused]] void attach(const char* title);

  inline FILE* stream;
}
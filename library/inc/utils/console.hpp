#pragma once

#include <cstdio>

namespace utils::console {
  void attach(const char* title);

  inline FILE* stream;
}

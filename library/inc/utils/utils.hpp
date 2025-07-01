#pragma once

#include <windows.h>

#include <array>

namespace utils {
  std::array<void*, 18> GetSwapChainVmt();
  DWORD GetPageSize();
}

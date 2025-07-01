#pragma once

#include <windows.h>

namespace utils {
  void* GetSwapChainMethod(size_t index);
  DWORD GetPageSize();
}

#include <utils/console.hpp>

#include <windows.h>
#include <cstdio>

void utils::console::attach(const char* title) {
  AllocConsole();
  const auto _ = freopen_s((FILE**)stdout, "conout$", "w", stdout);
  SetConsoleTitleA(title);
}

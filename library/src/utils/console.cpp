#include <utils/console.hpp>

#include <windows.h>

void utils::console::attach(const char* title) {
  AllocConsole();
  const auto _ = freopen_s(&utils::console::stream, "conout$", "w", stdout);
  SetConsoleTitleA(title);
}

#include <utils/console.hpp>

#include <windows.h>

[[maybe_unused]] void utils::console::attach(const char* title) {
  AllocConsole();
  freopen_s(&utils::console::stream, "conout$", "w", stdout);
  SetConsoleTitleA(title);
}
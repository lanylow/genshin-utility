#include <common.hpp>

[[maybe_unused]] void utils::console::attach(const char* title) {
  AllocConsole();
  freopen_s(&utils::console::stream, "conout$", "w", stdout);
  SetConsoleTitleA(title);
}
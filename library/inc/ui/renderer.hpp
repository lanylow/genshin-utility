#pragma once

#include <windows.h>

#include <imgui/imgui.h>

namespace ui::renderer {
  void initialize();
  long long handle_message(HWND window, unsigned int message, unsigned long long wparam, long long lparam);

  void begin();
  void end();

  void add_text(const char* name, ImVec2 pos, ImColor color);
  void add_rectangle(ImVec2 pos, ImVec2 size, ImColor color);
  void add_outlined_rectangle(ImVec2 pos, ImVec2 size, ImColor color);
}

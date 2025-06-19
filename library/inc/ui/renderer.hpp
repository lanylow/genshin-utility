#pragma once

#include <windows.h>

#include <imgui/imgui.h>

#include <hooks/hooks.hpp>

namespace ui::renderer {
  void initialize(const hooks::RenderData& render_data);
  LRESULT handle_message(HWND window, UINT msg, WPARAM wparam, LPARAM lparam);

  void begin();
  void end(const hooks::RenderData& render_data);

  void add_text(const char* name, ImVec2 pos, ImColor color);
  void add_rectangle(ImVec2 pos, ImVec2 size, ImColor color);
  void add_outlined_rectangle(ImVec2 pos, ImVec2 size, ImColor color);
}

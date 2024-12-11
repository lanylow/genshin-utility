#pragma once

#include <windows.h>
#include <string>

#include <imgui/imgui.h>

namespace ui::renderer {
  void initialize();
  long long handle_message(HWND window, unsigned int message, unsigned long long wparam, long long lparam);

  void begin();
  void end();

  void add_text(const char* name, ImVec2 pos, int r, int g, int b, int a);
  void add_rectangle(ImVec2 pos, ImVec2 size, int r, int g, int b, int a);
  void add_outlined_rectangle(ImVec2 pos, ImVec2 size, int r, int g, int b, int a);
}

namespace ui {
  class window {
  public:
    window(ImVec2 size, std::string  text_left, std::string  text_right);

    void begin();
    void add_groupbox(const char* name, float x, float y, float width, float height);
    void add_checkbox(const char* name, bool* item, bool grayed_out = false);
    void add_slider(const char* name, int min, int max, int* item, int step);

  private:
    std::string text_left;
    std::string text_right;

    ImVec2 size = ImVec2(324.f, 218.f);
    ImVec2 pos = ImVec2(100.f, 100.f);
    ImVec2 mouse_drag = ImVec2(300.f, 300.f);

    bool mouse_clicked = false;
    bool mouse_dragging = false;

    ImVec2 groupbox_offset;
    float groupbox_top = 0.f;
    float groupbox_bottom = 0.f;
  };
}
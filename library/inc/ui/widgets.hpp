#pragma once

#include <windows.h>

#include <string>

#include <imgui/imgui.h>

namespace ui::widgets {
  class groupbox {
  public:
    groupbox(ImVec2 offset, float top, float bottom);

    void add_checkbox(const char* name, bool* item, bool grayed_out = false);
    void add_slider(const char* name, int min, int max, int* item, int step);

  private:
    ImVec2 offset;
    float top = 0.f;
    float bottom = 0.f;
  };

  class window {
  public:
    window(ImVec2 size, std::string text_left, std::string text_right);

    void begin();
    widgets::groupbox add_groupbox(const char* name, float x, float y, float width, float height) const;

  private:
    ImVec2 size = {100.f, 100.f};
    ImVec2 pos = {100.f, 100.f};

    std::string text_left;
    std::string text_right;

    ImVec2 mouse_drag = {300.f, 300.f};
    bool mouse_clicked = false;
    bool mouse_dragging = false;
  };
}

#pragma once

#include <windows.h>

namespace ui {
  void initialize_imgui();
  long long handle_message(HWND window, unsigned int message, unsigned long long wparam, long long lparam);

  void begin();
  void end();

  void add_window(const char* name);
  void add_groupbox(const char* name, float x, float y, float width, float height);

  void add_checkbox(const char* name, bool* item);
  void add_slider(const char* name, int min, int max, int* item, int step);

  void add_text(const char* name, float x, float y, int r, int g, int b, int a);
  void add_rectangle(float x, float y, float width, float height, int r, int g, int b, int a);
  void add_outlined_rectangle(float x, float y, float width, float height, int r, int g, int b, int a);

  inline float menu_width = 324.f;
  inline float menu_height = 218.f;
  inline float position_x = 100.f;
  inline float position_y = 100.f;

  inline float mouse_drag_x = 300.f;
  inline float mouse_drag_y = 300.f;
  inline bool mouse_clicked;
  inline bool mouse_dragging;

  inline float groupbox_offset_x;
  inline float groupbox_offset_y;
  inline float groupbox_top;
  inline float groupbox_bottom;
}
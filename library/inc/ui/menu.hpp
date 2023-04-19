#pragma once

namespace ui::menu {
  void initialize();
  long long handle_message(HWND window, unsigned int message, unsigned long long wparam, long long lparam);
  void handle_frame();

  void render_menu();
  void render_counter();

  inline LARGE_INTEGER performance_counter;
  inline LARGE_INTEGER performance_frequency;
  inline int frames = 0;
  inline int frame_rate = 0;
}
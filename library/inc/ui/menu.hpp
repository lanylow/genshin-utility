#pragma once

namespace ui::menu {
  void initialize();
  long long handle_message(HWND window, unsigned int message, unsigned long long wparam, long long lparam);

  void render_menu();
  void render_counter();

  inline int frame_rate = 0;
}
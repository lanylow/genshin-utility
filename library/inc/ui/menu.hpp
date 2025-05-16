#pragma once

#include <windows.h>

#include <ui/widgets.hpp>

namespace ui {
  class menu {
  public:
    menu();

    void render();

    static long long handle_message(HWND window, uint32_t message, WPARAM wparam, LPARAM lparam);

  private:
    void render_menu();
    void render_fps_counter();
    void update_fps_counter();

    widgets::window window;
    LARGE_INTEGER performance_counter = {};
    LARGE_INTEGER performance_frequency = {};
    int frames = 0;
    int frame_rate = 0;
  };
}

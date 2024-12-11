#pragma once

#include <windows.h>

#include <ui/widgets.hpp>

namespace ui {
  class menu {
  public:
    menu();

    static long long handle_message(HWND window, uint32_t message, WPARAM wparam, LPARAM lparam);

    void render();

  private:
    void render_menu();
    void render_fps_counter();
    void update_fps_counter();

    widgets::window window;
    LARGE_INTEGER performance_counter = { 0 };
    LARGE_INTEGER performance_frequency = { 0 };
    int frames = 0;
    int frame_rate = 0;
  };
}
#pragma once

#include <windows.h>

#include <ui/widgets.hpp>

namespace ui {
  class menu {
  public:
    menu();

    static LRESULT handle_message(HWND window, UINT msg, WPARAM wparam, LPARAM lparam);
    
    void render();

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

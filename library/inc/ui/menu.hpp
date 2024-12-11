#pragma once

#include <windows.h>
#include <ui/ui.hpp>

namespace ui {
  class menu {
  public:
    menu();

    static long long handle_message(HWND window, unsigned int message, unsigned long long wparam, long long lparam);

    void render();

  private:
    void render_menu();
    void render_counter();

    ui::window window;
    LARGE_INTEGER performance_counter = { 0 };
    LARGE_INTEGER performance_frequency = { 0 };
    int frames = 0;
    int frame_rate = 0;
  };
}
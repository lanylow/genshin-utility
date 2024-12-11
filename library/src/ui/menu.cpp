#include <ui/options.hpp>
#include <ui/menu.hpp>
#include <ui/ui.hpp>
#include <sdk.hpp>

#include <imgui/imgui.h>

#include <format>

ui::menu::menu()
  : window({ 324.f, 218.f }, "Genshin Utility by lanylow", "v1.5.8") {
  QueryPerformanceFrequency(&performance_frequency);
  QueryPerformanceCounter(&performance_counter);
}

long long ui::menu::handle_message(HWND window, unsigned int message, unsigned long long wparam, long long lparam) {
  static bool insert = false;

  if (wparam == VK_INSERT) {
    if (message == WM_KEYDOWN && !insert) {
      ui::options::menu::opened = !ui::options::menu::opened;
      insert = true;
    }
    else if (message == WM_KEYUP) 
      insert = false;
  }

  return ui::renderer::handle_message(window, message, wparam, lparam);
}

void ui::menu::render() {
  frames++;

  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  auto delta = (double)(now.QuadPart - performance_counter.QuadPart) / (double)(performance_frequency.QuadPart);

  if (delta >= 1.0) {
    frame_rate = frames;
    frames = 0;
    QueryPerformanceCounter(&performance_counter);
  }

  render_menu();
  render_counter();
}

void ui::menu::render_menu() {
  if (!ui::options::menu::opened)
    return;

  window.begin();
  window.add_groupbox("Settings", 19, 20 + 18, 286, 162);
  window.add_checkbox("Open menu on start", &ui::options::menu::open_on_start);
  window.add_checkbox("Frame rate counter", &ui::options::tools::fps_counter);
  window.add_checkbox("Enable V-Sync", &ui::options::tools::enable_vsync);
  window.add_checkbox("Disable fog", &ui::options::tools::disable_fog, sdk::game_t::is(sdk::game_t::star_rail));
  window.add_slider("Frame rate limit", 10, 360, &ui::options::tools::fps_limit, 1);
  window.add_slider("Camera field of view", 5, 175, &ui::options::tools::camera_fov, 1);
}

void ui::menu::render_counter() {
  if (!ui::options::tools::fps_counter)
    return;

  auto text = std::format("{} fps", ui::menu::frame_rate);
  auto text_size = ImGui::CalcTextSize(text.c_str());

  auto screen_width = ImGui::GetMainViewport()->Size.x;
  auto text_begin = screen_width - 2 - 5 - text_size.x;
  auto rect_begin = text_begin - 5;
  auto rect_end = screen_width - (rect_begin) - 2;

  renderer::add_rectangle({ rect_begin, 2 }, { rect_end, 20 }, 50, 50, 50, 255);
  renderer::add_rectangle({ rect_begin, 21 }, { rect_end, 1 }, 195, 141, 145, 255);
  renderer::add_outlined_rectangle({ rect_begin, 2 }, { rect_end, 20 }, 0, 0, 0, 84);
  renderer::add_text(text.c_str(), { text_begin, 4 }, 180, 180, 180, 255);
}
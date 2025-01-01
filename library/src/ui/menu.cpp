#include <ui/menu.hpp>

#include <format>

#include <imgui/imgui.h>

#include <ui/renderer.hpp>
#include <options.hpp>
#include <sdk.hpp>

ui::menu::menu()
  : window({ 324.f, 218.f }, "Genshin Utility by lanylow", "v1.5.10") {
  QueryPerformanceFrequency(&performance_frequency);
  QueryPerformanceCounter(&performance_counter);
}

long long ui::menu::handle_message(HWND window, uint32_t message, WPARAM wparam, LPARAM lparam) {
  static bool insert = false;

  if (wparam == VK_INSERT) {
    if (message == WM_KEYDOWN && !insert) {
      options::menu.opened = !options::menu.opened;
      insert = true;
    }
    else if (message == WM_KEYUP) 
      insert = false;
  }

  return ui::renderer::handle_message(window, message, wparam, lparam);
}

void ui::menu::render() {
  update_fps_counter();
  render_menu();
  render_fps_counter();
}

void ui::menu::render_menu() {
  if (!options::menu.opened)
    return;

  window.begin();

  auto groupbox = window.add_groupbox("Settings", 19, 20 + 18, 286, 162);
  groupbox.add_checkbox("Open menu on start", &options::menu.open_on_start);
  groupbox.add_checkbox("Frame rate counter", &options::tools.fps_counter);
  groupbox.add_checkbox("Enable V-Sync", &options::tools.enable_vsync);
  groupbox.add_checkbox("Disable fog", &options::tools.disable_fog, sdk::game_t::is(sdk::game_t::star_rail));
  groupbox.add_slider("Frame rate limit", 10, 360, &options::tools.fps_limit, 1);
  groupbox.add_slider("Camera field of view", 5, 175, &options::tools.camera_fov, 1);
}

void ui::menu::render_fps_counter() {
  if (!options::tools.fps_counter)
    return;

  auto text = std::format("{} fps", frame_rate);
  auto text_size = ImGui::CalcTextSize(text.c_str());

  auto screen_width = ImGui::GetMainViewport()->Size.x;
  auto text_begin = screen_width - 2 - 5 - text_size.x;
  auto rect_begin = text_begin - 5;
  auto rect_end = screen_width - (rect_begin) - 2;

  renderer::add_rectangle({ rect_begin, 2 }, { rect_end, 20 }, { 50, 50, 50, 255 });
  renderer::add_rectangle({ rect_begin, 21 }, { rect_end, 1 }, { 195, 141, 145, 255 });
  renderer::add_outlined_rectangle({ rect_begin, 2 }, { rect_end, 20 }, { 0, 0, 0, 84 });
  renderer::add_text(text.c_str(), { text_begin, 4 }, { 180, 180, 180, 255 });
}

void ui::menu::update_fps_counter() {
  frames++;

  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  auto delta = (double)(now.QuadPart - performance_counter.QuadPart) / (double)(performance_frequency.QuadPart);

  if (delta < 1.0)
    return;

  frame_rate = frames;
  frames = 0;

  QueryPerformanceCounter(&performance_counter);
}
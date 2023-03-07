#include <common.hpp>

void ui::menu::initialize() {
  ui::gui::initialize_imgui();
}

long long ui::menu::handle_message(HWND window, unsigned int message, unsigned long long wparam, long long lparam) {
  static bool insert = false;

  if (wparam == VK_INSERT) {
    if (message == WM_KEYDOWN && !insert) {
      variables::menu::opened = !variables::menu::opened;
      insert = true;
    }
    else if (message == WM_KEYUP) 
      insert = false;
  }

  return ui::gui::handle_message(window, message, wparam, lparam);
}

void ui::menu::render_menu() {
  if (!variables::menu::opened)
    return;

  ui::gui::add_window("Genshin Utility by lanylow");
  ui::gui::add_groupbox("Settings", 19, 20 + 18, 286, 162);
  ui::gui::add_checkbox("Open menu on start", &variables::menu::open_on_start);
  ui::gui::add_checkbox("Frame rate counter", &variables::tools::fps_counter);
  ui::gui::add_checkbox("Enable V-Sync", &variables::tools::enable_vsync);
  ui::gui::add_checkbox("Disable fog", &variables::tools::disable_fog);
  ui::gui::add_slider("Frame rate limit", 10, 360, &variables::tools::fps_limit, 1);
  ui::gui::add_slider("Camera field of view", 5, 175, &variables::tools::camera_fov, 1);
}

void ui::menu::render_counter() {
  if (!variables::tools::fps_counter)
    return;

  auto screen_width = ImGui::GetMainViewport()->Size.x;
  auto text = "Frame rate: " + std::to_string(ui::menu::frame_rate);

  ui::gui::add_rectangle(screen_width - 102, 2, 100, 20, 50, 50, 50, 255);
  ui::gui::add_rectangle(screen_width - 102, 21, 100, 1, 195, 141, 145, 255);
  ui::gui::add_outlined_rectangle(screen_width - 102, 2, 100, 20, 0, 0, 0, 84);
  ui::gui::add_text(text.c_str(), screen_width - 97, 4, 180, 180, 180, 255);
}
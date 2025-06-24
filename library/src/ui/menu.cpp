#include <ui/menu.hpp>

#include <format>

#include <ui/renderer.hpp>
#include <sdk.hpp>

Menu::Menu(Renderer* renderer)
  : window_(renderer, ImVec2(324.f, 218.f), "Genshin Utility by lanylow", "v1.5.19")
  , renderer_(renderer) {}

void Menu::ReadConfig() {
  config_file_manager_.ReadConfig(config_);

  if (config_.menu.open_on_start)
    ToggleMenu();
}

void Menu::WriteConfig() {
  config_file_manager_.WriteConfig(config_);
}

void Menu::Render() {
  RenderMenu();
  RenderFpsCounter();
}

void Menu::ToggleMenu() {
  config_.menu.opened ^= true;
  renderer_->GetRenderData().capture_input = config_.menu.opened;
}

void Menu::RenderMenu() {
  if (ImGui::IsKeyPressed(ImGuiKey_Insert))
    ToggleMenu();

  if (!config_.menu.opened)
    return;

  window_.Begin();

  auto groupbox = window_.AddGroupbox("Settings", 19, 20 + 18, 286, 162);
  groupbox.AddCheckbox("Open menu on start", config_.menu.open_on_start);
  groupbox.AddCheckbox("Frame rate counter", config_.tools.fps_counter);
  groupbox.AddCheckbox("Enable V-Sync", config_.tools.enable_vsync, sdk::game_t::is(sdk::game_t::star_rail));
  groupbox.AddCheckbox("Disable fog", config_.tools.disable_fog, sdk::game_t::is(sdk::game_t::star_rail));
  groupbox.AddSlider("Frame rate limit", config_.tools.fps_limit, 10, 360, 1);
  groupbox.AddSlider("Camera field of view", config_.tools.camera_fov, 5, 175, 1);
}

void Menu::RenderFpsCounter() {
  UpdateFpsCounter();

  if (!config_.tools.fps_counter)
    return;

  const auto text = std::format("{} fps", frame_rate_);
  const auto text_size = ImGui::CalcTextSize(text.c_str());

  const auto screen_width = ImGui::GetMainViewport()->Size.x;
  const auto text_begin = screen_width - 2 - 5 - text_size.x;
  const auto rect_begin = text_begin - 5;
  const auto rect_end = screen_width - rect_begin - 2;

  renderer_->AddRectangle(ImVec2(rect_begin, 2), ImVec2(rect_end, 20), ImColor(50, 50, 50));
  renderer_->AddRectangle(ImVec2(rect_begin, 21), ImVec2(rect_end, 1), ImColor(195, 141, 145));
  renderer_->AddOutlinedRectangle(ImVec2(rect_begin, 2), ImVec2(rect_end, 20), ImColor(0, 0, 0, 84));
  renderer_->AddText(text, ImVec2(text_begin, 4), ImColor(180, 180, 180));
}

void Menu::UpdateFpsCounter() {
  frames_++;

  const auto now = std::chrono::high_resolution_clock::now();
  const auto elapsed = std::chrono::duration<double>(now - start_).count();

  if (elapsed < 1.0)
    return;

  start_ = now;
  frame_rate_ = frames_;
  frames_ = 0;
}

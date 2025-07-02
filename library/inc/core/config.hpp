#pragma once

#include <filesystem>

#include <imgui/imgui.h>

struct MenuConfig {
  ImGuiKey_ key = ImGuiKey_Insert;
  bool open_on_start = true;
};

struct ToolsConfig {
  bool fps_counter = true;
  bool enable_vsync = false;
  bool disable_fog = false;
  int fps_limit = 120;
  int camera_fov = 45;
};

struct Config {
  MenuConfig menu;
  ToolsConfig tools;
};

class ConfigFileManager {
public:
  ConfigFileManager();

  void ReadConfig(Config& config) const;
  void WriteConfig(const Config& config) const;

private:
  std::filesystem::path folder_path_;
  std::filesystem::path file_path_;
};

#pragma once

#include <filesystem>

struct MenuConfig {
  bool opened = false;
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

  void ReadConfig(Config& config);
  void WriteConfig(const Config& config);

private:
  void InitPaths();

  std::filesystem::path folder_path_;
  std::filesystem::path file_path_;
};

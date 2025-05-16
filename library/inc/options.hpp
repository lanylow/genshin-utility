#pragma once

#include <filesystem>

namespace options {
  struct menu_t {
    bool opened = true;
    bool open_on_start = true;
  };

  struct tools_t {
    bool fps_counter = false;
    bool enable_vsync = false;
    bool disable_fog = false;
    int fps_limit = 120;
    int camera_fov = 45;
  };
}

namespace options {
  void load();
  void save();
  std::filesystem::path get_working_path();

  inline options::menu_t menu;
  inline options::tools_t tools;

  inline const std::filesystem::path folder = options::get_working_path();
  inline const std::filesystem::path path = options::folder / "config.lff";
}

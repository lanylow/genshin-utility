#pragma once

#include <filesystem>

namespace ui::options {
  void load();
  void save();
  std::filesystem::path get_working_path();

  inline const std::filesystem::path folder = ui::options::get_working_path();
  inline const std::filesystem::path path = ui::options::folder / "config.lff";
}

namespace ui::options::menu {
  inline bool opened = true;
  inline bool open_on_start = true;
}

namespace ui::options::tools {
  inline bool fps_counter = false;
  inline bool enable_vsync = false;
  inline bool disable_fog = false;
  inline int fps_limit = 120;
  inline int camera_fov = 45;
}
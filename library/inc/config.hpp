#pragma once

namespace config {
  void load();
  void save();
  std::filesystem::path get_working_path();

  inline const std::filesystem::path config_folder = get_working_path();
  inline const std::filesystem::path config_path = config_folder / "config.gu";
}
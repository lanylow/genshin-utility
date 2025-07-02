#include <core/config.hpp>

#include <shlobj.h>

#define MINI_CASE_SENSITIVE
#include <mini/ini.h>

namespace {
  template <typename ValueType>
  void Read(mINI::INIStructure& ini, const std::string& section, const std::string& key, ValueType& value) {
    auto str = std::string();

    if (ini.has(section))
      if (auto& keys = ini[section]; keys.has(key))
        str = keys[key];

    if (str.empty())
      return;

    if constexpr (std::same_as<ValueType, int>)
      value = std::stoi(str);
    else if constexpr (std::same_as<ValueType, bool>)
      value = str == "True";
  }

  template <typename ValueType>
  void Write(mINI::INIStructure& ini, const std::string& section, const std::string& key, ValueType value) {
    if constexpr (std::same_as<ValueType, int>)
      ini[section][key] = std::to_string(value);
    else if constexpr (std::same_as<ValueType, bool>)
      ini[section][key] = value ? "True" : "False";
  }
}

ConfigFileManager::ConfigFileManager() {
  wchar_t* path_to_documents;

  if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &path_to_documents)))
    folder_path_ = path_to_documents;

  CoTaskMemFree(path_to_documents);

  if (folder_path_.empty())
    return;

  folder_path_ /= "genshin-utility";

  if (!std::filesystem::is_directory(folder_path_)) {
    std::filesystem::remove(folder_path_);

    if (!std::filesystem::create_directories(folder_path_))
      return;
  }

  file_path_ = folder_path_ / "config.ini";
}

void ConfigFileManager::ReadConfig(Config& config) const {
  if (file_path_.empty())
    return;

  auto ini = mINI::INIStructure();

  if (const auto file = mINI::INIFile(file_path_); !file.read(ini))
    return;

  Read(ini, "Menu", "OpenOnStart", config.menu.open_on_start);
  Read(ini, "Tools", "FpsCounter", config.tools.fps_counter);
  Read(ini, "Tools", "EnableVSync", config.tools.enable_vsync);
  Read(ini, "Tools", "DisableFog", config.tools.disable_fog);
  Read(ini, "Tools", "FpsLimit", config.tools.fps_limit);
  Read(ini, "Tools", "CameraFov", config.tools.camera_fov);
}

void ConfigFileManager::WriteConfig(const Config& config) const {
  if (file_path_.empty())
    return;

  auto ini = mINI::INIStructure();

  Write(ini, "Menu", "OpenOnStart", config.menu.open_on_start);
  Write(ini, "Tools", "FpsCounter", config.tools.fps_counter);
  Write(ini, "Tools", "EnableVSync", config.tools.enable_vsync);
  Write(ini, "Tools", "DisableFog", config.tools.disable_fog);
  Write(ini, "Tools", "FpsLimit", config.tools.fps_limit);
  Write(ini, "Tools", "CameraFov", config.tools.camera_fov);

  const auto file = mINI::INIFile(file_path_);
  file.write(ini, true);
}

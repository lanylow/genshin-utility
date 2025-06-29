#include <config.hpp>

#include <shlobj.h>

#define MINI_CASE_SENSITIVE
#include <mini/ini.h>

namespace {
  std::string TryRead(mINI::INIStructure& ini, const std::string& section, const std::string& key) {
    if (ini.has(section))
      if (auto& keys = ini[section]; keys.has(key))
        return keys[key];

    return std::string();
  }

  void TryRead(mINI::INIStructure& ini, const std::string& section, const std::string& key, int& value) {
    if (const auto str = TryRead(ini, section, key); !str.empty())
      value = std::stoi(str);
  }

  void TryRead(mINI::INIStructure& ini, const std::string& section, const std::string& key, bool& value) {
    if (const auto str = TryRead(ini, section, key); !str.empty())
      value = str == "True";
  }

  void Write(mINI::INIStructure& ini, const std::string& section, const std::string& key, int value) {
    ini[section][key] = std::to_string(value);
  }

  void Write(mINI::INIStructure& ini, const std::string& section, const std::string& key, bool value) {
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

void ConfigFileManager::ReadConfig(Config& config) {
  if (file_path_.empty())
    return;

  auto ini = mINI::INIStructure();

  if (const auto file = mINI::INIFile(file_path_); !file.read(ini))
    return;

  TryRead(ini, "Menu", "OpenOnStart", config.menu.open_on_start);
  TryRead(ini, "Tools", "FpsCounter", config.tools.fps_counter);
  TryRead(ini, "Tools", "EnableVSync", config.tools.enable_vsync);
  TryRead(ini, "Tools", "DisableFog", config.tools.disable_fog);
  TryRead(ini, "Tools", "FpsLimit", config.tools.fps_limit);
  TryRead(ini, "Tools", "CameraFov", config.tools.camera_fov);
}

void ConfigFileManager::WriteConfig(const Config& config) {
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

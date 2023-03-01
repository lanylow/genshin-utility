#include <common.hpp>
#include <json/json.hpp>

namespace GenshinUtility {
  void GConfig::Load() noexcept {
    if (!fs::is_directory(m_configFolder)) {
      fs::remove(m_configFolder);

      if (!fs::create_directories(m_configFolder))
        return;
    }

    std::ifstream inputFile(m_configPath.string(), std::ios::in);

    if (!inputFile.good())
      return;

    nlohmann::json config = { };

    try {
      config = nlohmann::json::parse(inputFile, nullptr, false);

      if (config.is_discarded())
        return;

      inputFile.close();
    }
    catch (std::ifstream::failure&) {
      return;
    }

    try {
      Options.openMenuOnStart = config["openMenuOnStart"];
      Options.fpsCounter = config["fpsCounter"];
      Options.enableVSync = config["enableVSync"];
      Options.disableFog = config["disableFog"];
      Options.fpsLimit = config["fpsLimit"];
      Options.cameraFov = config["cameraFov"];
    }
    catch (nlohmann::detail::exception&) {
      // ignored
    }
  }

  void GConfig::Save() noexcept {
    nlohmann::json config = { };

    try {
      config["openMenuOnStart"] = Options.openMenuOnStart;
      config["fpsCounter"] = Options.fpsCounter;
      config["enableVSync"] = Options.enableVSync;
      config["disableFog"] = Options.disableFog;
      config["fpsLimit"] = Options.fpsLimit;
      config["cameraFov"] = Options.cameraFov;
    }
    catch (nlohmann::detail::exception&) {
      return;
    }

    std::ofstream outputFile(m_configPath.string(), std::ios::out | std::ios::trunc);

    if (!outputFile.good())
      return;

    try {
      outputFile << config.dump();
      outputFile.close();
    }
    catch (std::ofstream::failure&) {
      // ignored
    }
  }

  fs::path GConfig::GetWorkingPath() noexcept {
    fs::path path;

    if (wchar_t* pathToDocuments; SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &pathToDocuments))) {
      path.assign(pathToDocuments);
      path.append("genshin-utility");
      CoTaskMemFree(pathToDocuments);
    }

    return path;
  }
}
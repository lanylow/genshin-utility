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
      Options.fpsCounter = config["fpsCounter"].get<bool>();
      Options.enableVSync = config["enableVSync"].get<bool>();
      Options.disableFog = config["disableFog"].get<bool>();
      Options.fpsLimit = config["fpsLimit"].get<int>();
      Options.cameraFov = config["cameraFov"].get<int>();
    }
    catch (nlohmann::detail::exception&) {
      // ignored
    }
  }

  void GConfig::Save() noexcept {
    nlohmann::json config = { };

    try {
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
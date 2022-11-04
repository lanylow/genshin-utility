#pragma once

namespace GenshinUtility {
  class GConfig final : public GInstance<GConfig> {
  public:
    void Load() noexcept;
    void Save() noexcept;
    fs::path GetWorkingPath() noexcept;

    const fs::path m_configFolder = GetWorkingPath();
    const fs::path m_configPath = m_configFolder / "config.gu";
  };
}
#include <common.hpp>

namespace GenshinUtility {
  void GMenu::Render() noexcept {
    if (!Options.menuOpened)
      return;

    m_gui.AddWindow("Genshin Utility by lanylow");
    m_gui.AddGroupbox(19, 20 + 18, 286, 162, "Settings");
    m_gui.AddCheckbox("Open menu on start", &Options.openMenuOnStart);
    m_gui.AddCheckbox("Frame rate counter", &Options.fpsCounter);
    m_gui.AddCheckbox("Enable V-Sync", &Options.enableVSync);
    m_gui.AddCheckbox("Disable fog", &Options.disableFog);
    m_gui.AddSlider(10, 360, "Frame rate limit", &Options.fpsLimit, 1);
    m_gui.AddSlider(5, 175, "Camera field of view", &Options.cameraFov, 1);
  }
}
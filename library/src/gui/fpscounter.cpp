#include <common.hpp>

namespace GenshinUtility {
  void GFpsCounter::Render() noexcept {
    if (!Options.fpsCounter)
      return;

    ImGuiViewport* viewport = ImGui::GetMainViewport();

    float screenWidth = viewport->Size.x;
    float screenHeight = viewport->Size.y;

    std::string frameRateText = "Frame rate: " + std::to_string(m_frameRate);

    m_gui.AddRectangle(screenWidth - 102, 2, 100, 20, 50, 50, 50, 255);
    m_gui.AddRectangle(screenWidth - 102, 21, 100, 1, 195, 141, 145, 255);
    m_gui.AddOutlinedRectangle(screenWidth - 102, 2, 100, 20, 0, 0, 0, 84);
    m_gui.AddText(screenWidth - 97, 4, 180, 180, 180, 255, frameRateText.c_str());
  }

  void GFpsCounter::SetFrameRate(int frameRate) noexcept {
    m_frameRate = frameRate;
  }
}
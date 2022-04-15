#pragma once

namespace GenshinUtility {
  class GFpsCounter final : public GInstance<GFpsCounter> {
  public:
    void Render() noexcept;
    void SetFrameRate(int frameRate) noexcept;

  private:
    FGui m_gui;
    int m_frameRate;
  };
}
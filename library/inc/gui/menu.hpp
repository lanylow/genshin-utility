#pragma once

namespace GenshinUtility {
  class GMenu final : public GInstance<GMenu> {
  public:
    void Render() noexcept;

  private:
    FGui m_gui;
  };
}
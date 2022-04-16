#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

namespace GenshinUtility {
  class FGui final {
  public:
    void AddWindow(const char* name) noexcept;
    void AddCheckbox(const char* name, bool* item) noexcept;
    void AddSlider(int min, int max, const char* name, int* item, int step) noexcept;
    void AddGroupbox(float x, float y, float width, float height, const char* name) noexcept;
    void AddText(float x, float y, int r, int g, int b, int a, const char* text) noexcept;

    void AddRectangle(float x, float y, float width, float height, int r, int g, int b, int a) const noexcept;
    void AddOutlinedRectangle(float x, float y, float width, float height, int r, int g, int b, int a) const noexcept;

  private:
    float m_menuWidth = 324.f;
    float m_menuHeight = 200.f;
    float m_positionX = 100.f;
    float m_positionY = 100.f;
    float m_mouseDragX = 300.f;
    float m_mouseDragY = 300.f;
    bool m_mouseClicked;
    bool m_mouseDragging;
    float m_groupBoxOffsetX;
    float m_groupBoxOffsetY;
    float m_groupBoxTop;
    float m_groupBoxBottom;
    float m_groupBoxWidth;
  };
}
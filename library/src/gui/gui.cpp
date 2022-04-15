#include <common.hpp>

namespace GenshinUtility {
  void FGui::AddWindow(const char* name) noexcept {
    m_mouseClicked = ImGui::IsMouseDown(ImGuiMouseButton_Left);
    ImVec2 mousePosition = ImGui::GetMousePos();

    if (m_mouseDragging && !m_mouseClicked)
      m_mouseDragging = false;

    if (m_mouseDragging && m_mouseClicked) {
      m_positionX = mousePosition.x - m_mouseDragX;
      m_positionY = mousePosition.y - m_mouseDragY;
    }

    if (ImGui::IsMouseHoveringRect({ m_positionX, m_positionY }, { m_positionX + m_menuWidth, m_positionY + 20 })) {
      m_mouseDragging = true;
      m_mouseDragX = mousePosition.x - m_positionX;
      m_mouseDragY = mousePosition.y - m_positionY;
    }

    AddRectangle(m_positionX, m_positionY, m_menuWidth, m_menuHeight, 50, 50, 50, 255);
    AddRectangle(m_positionX, m_positionY + 20, m_menuWidth, 1, 195, 141, 145, 255);
    AddOutlinedRectangle(m_positionX, m_positionY, m_menuWidth, m_menuHeight, 0, 0, 0, 85);
    AddText(m_positionX + 5, m_positionY + 2, 180, 180, 180, 255, name);
  }

  void FGui::AddCheckbox(const char* name, bool* item) noexcept {
    if (m_groupBoxBottom <= m_groupBoxOffsetY + 18)
      return;

    if (m_groupBoxTop >= m_groupBoxOffsetY + 18)
      return;

    ImVec2 textSize = ImGui::CalcTextSize(name);

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect({ m_groupBoxOffsetX + 7, m_groupBoxOffsetY }, { m_groupBoxOffsetX + 7 + 12 + textSize.x, m_groupBoxOffsetY + 12 }))
      *item = !*item;

    if (*item)
      AddRectangle(m_groupBoxOffsetX + 8, m_groupBoxOffsetY, 12, 12, 195, 141, 145, 255);
    else
      AddRectangle(m_groupBoxOffsetX + 8, m_groupBoxOffsetY, 12, 12, 50, 50, 50, 255);

    AddOutlinedRectangle(m_groupBoxOffsetX + 8, m_groupBoxOffsetY, 12, 12, 101, 101, 101, 255);
    AddText(m_groupBoxOffsetX + 29, m_groupBoxOffsetY - 2, 185, 185, 185, 255, name);

    if (ImGui::IsMouseHoveringRect({ m_groupBoxOffsetX + 7, m_groupBoxOffsetY }, { m_groupBoxOffsetX + 7 + 12 + textSize.x, m_groupBoxOffsetY + 12 }))
      AddOutlinedRectangle(m_groupBoxOffsetX + 9, m_groupBoxOffsetY + 1, 10, 10, 101, 101, 101, 255);

    m_groupBoxOffsetY += 18;
  }

  void FGui::AddSlider(int min, int max, const char* name, int* item, int step) noexcept {
    if (m_groupBoxBottom <= m_groupBoxOffsetY + 30)
      return;

    float pixelSize = (max - min) / 200.f;

    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect({ m_groupBoxOffsetX + 29, m_groupBoxOffsetY + 14 }, { m_groupBoxOffsetX + 29 + 201, m_groupBoxOffsetY + 14 + 10 })) {
      ImVec2 mousePosition = ImGui::GetMousePos();
      *item = static_cast<int>(abs(static_cast<int>(mousePosition.x - (m_groupBoxOffsetX + 29))) * pixelSize);
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect({ m_groupBoxOffsetX + 21, m_groupBoxOffsetY + 12 }, { m_groupBoxOffsetX + 21 + 8, m_groupBoxOffsetY + 12 + 10 }))
      *item -= step;

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect({ m_groupBoxOffsetX + 22 + 200, m_groupBoxOffsetY + 12 }, { m_groupBoxOffsetX + 22 + 200 + 15, m_groupBoxOffsetY + 12 + 10 }))
      *item += step;

    if (*item > max)
      *item = max;

    if (*item < min)
      *item = min;

    std::string displayText = std::string(name) + " [" + std::to_string(*item) + "]";

    AddText(m_groupBoxOffsetX + 29, m_groupBoxOffsetY - 3, 185, 185, 185, 255, displayText.c_str());
    AddRectangle(m_groupBoxOffsetX + 29, m_groupBoxOffsetY + 14, 200, 10, 50, 50, 50, 255);

    if (*item) {
      if (*item == max)
        AddRectangle(m_groupBoxOffsetX + 29, m_groupBoxOffsetY + 14, 200, 10, 195, 141, 145, 255);
      else
        AddRectangle(m_groupBoxOffsetX + 29, m_groupBoxOffsetY + 14, *item / pixelSize, 10, 195, 141, 145, 255);
    }

    AddOutlinedRectangle(m_groupBoxOffsetX + 29, m_groupBoxOffsetY + 14, 200, 10, 101, 101, 101, 255);
    AddText(m_groupBoxOffsetX + 22, m_groupBoxOffsetY + 10, 185, 185, 185, 255, "-");
    AddText(m_groupBoxOffsetX + 22 + 200 + 12, m_groupBoxOffsetY + 11, 185, 185, 185, 255, "+");

    if (ImGui::IsMouseHoveringRect({ m_groupBoxOffsetX + 29, m_groupBoxOffsetY + 14 }, { m_groupBoxOffsetX + 29 + 200 + 1, m_groupBoxOffsetY + 14 + 10 })) {
      ImVec2 mousePosition = ImGui::GetMousePos();

      int hoveredValue = static_cast<int>(abs(static_cast<int>(mousePosition.x - (m_groupBoxOffsetX + 29))) * pixelSize);
      AddRectangle(m_groupBoxOffsetX + 29, m_groupBoxOffsetY + 14, hoveredValue / pixelSize, 10, 101, 101, 101, 255);
      AddText(mousePosition.x + 12, mousePosition.y - 5, 185, 185, 185, 255, std::to_string(hoveredValue).c_str());
    }

    m_groupBoxOffsetY += 30;
  }

  void FGui::AddGroupbox(float x, float y, float width, float height, const char* name) noexcept {
    ImVec2 textSize = ImGui::CalcTextSize(name);

    AddOutlinedRectangle(m_positionX + x, m_positionY + y, width, height, 20, 20, 20, 255);
    AddOutlinedRectangle(m_positionX + x + 1, m_positionY + y + 1, width - 2, height - 2, 70, 70, 70, 255);
    AddRectangle(m_positionX + x + width / 2 - textSize.x / 2 - 3, m_positionY + y, textSize.x + 6, 2, 44, 44, 44, 255);
    AddText(m_positionX + x + width / 2 - textSize.x / 2, m_positionY + y - 6, 185, 185, 185, 255, name);

    m_groupBoxOffsetY = m_positionY + y + 12;
    m_groupBoxOffsetX = m_positionX + x;
    m_groupBoxBottom = m_positionY + y + height;
    m_groupBoxTop = m_positionY + y;
    m_groupBoxWidth = m_positionX + x + width;
  }

  void FGui::AddText(float x, float y, int r, int g, int b, int a, const char* text) noexcept {
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    drawList->AddText({ x + 1, y + 1 }, ImColor{ 0, 0, 0, 255 }, text);
    drawList->AddText({ x - 1, y - 1 }, ImColor{ 0, 0, 0, 255 }, text);
    drawList->AddText({ x + 1, y - 1 }, ImColor{ 0, 0, 0, 255 }, text);
    drawList->AddText({ x - 1, y + 1 }, ImColor{ 0, 0, 0, 255 }, text);

    drawList->AddText({ x + 1, y }, ImColor{ 0, 0, 0, 255 }, text);
    drawList->AddText({ x - 1, y }, ImColor{ 0, 0, 0, 255 }, text);
    drawList->AddText({ x, y - 1 }, ImColor{ 0, 0, 0, 255 }, text);
    drawList->AddText({ x, y + 1 }, ImColor{ 0, 0, 0, 255 }, text);

    drawList->AddText({ x, y }, ImColor{ r, g, b, a }, text);
  }

  void FGui::AddRectangle(float x, float y, float width, float height, int r, int g, int b, int a) const noexcept {
    ImGui::GetBackgroundDrawList()->AddRectFilled({ x, y }, { x + width, y + height }, ImColor{ r, g, b, a });
  }

  void FGui::AddOutlinedRectangle(float x, float y, float width, float height, int r, int g, int b, int a) const noexcept {
    ImGui::GetBackgroundDrawList()->AddRect({ x, y }, { x + width, y + height }, ImColor{ r, g, b, a });
  }
}
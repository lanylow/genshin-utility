#include <ui/widgets.hpp>

#include <algorithm>
#include <format>
#include <string>
#include <utility>

#include <imgui/imgui.h>

#include <ui/renderer.hpp>

ui::widgets::groupbox::groupbox(ImVec2 offset, float top, float bottom)
  : offset(offset), top(top), bottom(bottom) {}

void ui::widgets::groupbox::add_checkbox(const char* name, bool* item, bool grayed_out) {
  if (bottom <= offset.y + 18)
    return;

  if (top >= offset.y + 18)
    return;

  const auto text_size = ImGui::CalcTextSize(name);

  if (!grayed_out && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect({offset.x + 7, offset.y}, {offset.x + 7 + 12 + text_size.x, offset.y + 12}))
    *item = !*item;

  if (grayed_out) {
    renderer::add_rectangle({offset.x + 8, offset.y}, {12, 12}, {101, 101, 101, 255});
    renderer::add_text(name, {offset.x + 29, offset.y - 2}, {101, 101, 101, 255});
  }
  else {
    if (*item)
      renderer::add_rectangle({offset.x + 8, offset.y}, {12, 12}, {195, 141, 145, 255});
    else
      renderer::add_rectangle({offset.x + 8, offset.y}, {12, 12}, {50, 50, 50, 255});

    renderer::add_outlined_rectangle({offset.x + 8, offset.y}, {12, 12}, {101, 101, 101, 255});
    renderer::add_text(name, {offset.x + 29, offset.y - 2}, {185, 185, 185, 255});

    if (ImGui::IsMouseHoveringRect({offset.x + 7, offset.y}, {offset.x + 7 + 12 + text_size.x, offset.y + 12}))
      renderer::add_outlined_rectangle({offset.x + 9, offset.y + 1}, {10, 10}, {101, 101, 101, 255});
  }

  offset.y += 18;
}

void ui::widgets::groupbox::add_slider(const char* name, int min, int max, int* item, int step) {
  if (bottom <= offset.y + 30)
    return;

  const auto pixel_size = (float)(max - min) / 200.f;
  const auto mouse_pos = ImGui::GetMousePos();
  const auto hovered_value = (int)((float)std::abs((int)(mouse_pos.x - (offset.x + 29))) * pixel_size) + min;

  if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect({offset.x + 29, offset.y + 14}, {offset.x + 29 + 201, offset.y + 14 + 10}))
    *item = hovered_value;

  if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect({offset.x + 21, offset.y + 12}, {offset.x + 21 + 8, offset.y + 12 + 10}))
    *item -= step;

  if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect({offset.x + 22 + 200, offset.y + 12}, {offset.x + 22 + 200 + 15, offset.y + 12 + 10}))
    *item += step;

  *item = (std::min)(*item, max);
  *item = (std::max)(*item, min);

  const auto text = std::format("{} [{}]", name, *item);
  renderer::add_text(text.c_str(), {offset.x + 29, offset.y - 3}, {185, 185, 185, 255});
  renderer::add_rectangle({offset.x + 29, offset.y + 14}, {200, 10}, {50, 50, 50, 255});

  if (*item)
    renderer::add_rectangle({offset.x + 29, offset.y + 14}, {(float)(*item - min) / pixel_size, 10}, {195, 141, 145, 255});

  renderer::add_outlined_rectangle({offset.x + 29, offset.y + 14}, {200, 10}, {101, 101, 101, 255});
  renderer::add_text("-", {offset.x + 22, offset.y + 10}, {185, 185, 185, 255});
  renderer::add_text("+", {offset.x + 22 + 200 + 12, offset.y + 11}, {185, 185, 185, 255});

  if (ImGui::IsMouseHoveringRect({offset.x + 29, offset.y + 14}, {offset.x + 29 + 200 + 1, offset.y + 14 + 10})) {
    renderer::add_rectangle({offset.x + 29, offset.y + 14}, {(float)(hovered_value - min) / pixel_size, 10}, {101, 101, 101, 255});
    renderer::add_text(std::to_string(hovered_value).c_str(), {mouse_pos.x + 12, mouse_pos.y - 5}, {185, 185, 185, 255});
  }

  offset.y += 30;
}

ui::widgets::window::window(ImVec2 size, std::string text_left, std::string text_right)
  : size(size), text_left(std::move(text_left)), text_right(std::move(text_right)) {}

void ui::widgets::window::begin() {
  mouse_clicked = ImGui::IsMouseDown(ImGuiMouseButton_Left);
  const auto mouse_pos = ImGui::GetMousePos();

  if (mouse_dragging && !mouse_clicked)
    mouse_dragging = false;

  if (mouse_dragging && mouse_clicked) {
    pos.x = mouse_pos.x - mouse_drag.x;
    pos.y = mouse_pos.y - mouse_drag.y;
  }

  if (ImGui::IsMouseHoveringRect(pos, {pos.x + size.x, pos.y + 20})) {
    mouse_dragging = true;
    mouse_drag.x = mouse_pos.x - pos.x;
    mouse_drag.y = mouse_pos.y - pos.y;
  }

  renderer::add_rectangle(pos, size, {50, 50, 50, 255});
  renderer::add_rectangle({pos.x, pos.y + 20}, {size.x, 1}, {195, 141, 146, 255});
  renderer::add_outlined_rectangle(pos, size, {0, 0, 0, 85});

  renderer::add_text(text_left.c_str(), {pos.x + 5, pos.y + 2}, {180, 180, 180, 255});

  const auto text_right_size = ImGui::CalcTextSize(text_right.c_str());
  renderer::add_text(text_right.c_str(), {pos.x + size.x - text_right_size.x - 5, pos.y + 2}, {180, 180, 180, 255});
}

ui::widgets::groupbox ui::widgets::window::add_groupbox(const char* name, float x, float y, float width, float height) const {
  const auto text_size = ImGui::CalcTextSize(name);

  renderer::add_outlined_rectangle({pos.x + x, pos.y + y}, {width, height}, {20, 20, 20, 255});
  renderer::add_outlined_rectangle({pos.x + x + 1, pos.y + y + 1}, {width - 2, height - 2}, {70, 70, 70, 255});
  renderer::add_rectangle({pos.x + x + width / 2 - text_size.x / 2 - 3, pos.y + y}, {text_size.x + 6, 2}, {44, 44, 44, 255});
  renderer::add_text(name, {pos.x + x + width / 2 - text_size.x / 2, pos.y + y - 6}, {185, 185, 185, 255});

  return {{pos.x + x, pos.y + y + 12}, pos.y + y, pos.y + y + height};
}

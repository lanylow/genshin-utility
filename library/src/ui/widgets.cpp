#include <ui/widgets.hpp>

#include <algorithm>
#include <format>
#include <string>
#include <utility>

#include <ui/renderer.hpp>

Groupbox::Groupbox(Renderer* renderer, ImVec2 offset, float top, float bottom)
  : renderer_(renderer)
  , offset_(offset)
  , top_(top)
  , bottom_(bottom) {}

void Groupbox::AddCheckbox(const std::string& name, bool& val, bool grayed_out) {
  if (bottom_ <= offset_.y + 18)
    return;

  if (top_ >= offset_.y + 18)
    return;

  const auto text_size = ImGui::CalcTextSize(name.c_str());

  if (!grayed_out && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(ImVec2(offset_.x + 7, offset_.y), ImVec2(offset_.x + 7 + 12 + text_size.x, offset_.y + 12)))
    val ^= true;

  if (grayed_out) {
    renderer_->AddRectangle(ImVec2(offset_.x + 8, offset_.y), ImVec2(12, 12), ImColor(101, 101, 101));
    renderer_->AddText(name, ImVec2(offset_.x + 29, offset_.y - 2), ImColor(101, 101, 101));
  }
  else {
    if (val)
      renderer_->AddRectangle(ImVec2(offset_.x + 8, offset_.y), ImVec2(12, 12), ImColor(195, 141, 145));
    else
      renderer_->AddRectangle(ImVec2(offset_.x + 8, offset_.y), ImVec2(12, 12), ImColor(50, 50, 50));

    renderer_->AddOutlinedRectangle(ImVec2(offset_.x + 8, offset_.y), ImVec2(12, 12), ImColor(101, 101, 101));
    renderer_->AddText(name, ImVec2(offset_.x + 29, offset_.y - 2), ImColor(185, 185, 185));

    if (ImGui::IsMouseHoveringRect(ImVec2(offset_.x + 7, offset_.y), ImVec2(offset_.x + 7 + 12 + text_size.x, offset_.y + 12)))
      renderer_->AddOutlinedRectangle(ImVec2(offset_.x + 9, offset_.y + 1), ImVec2(10, 10), ImColor(101, 101, 101));
  }

  offset_.y += 18;
}

void Groupbox::AddSlider(const std::string& name, int& val, int min, int max, int step) {
  if (bottom_ <= offset_.y + 30)
    return;

  const auto pixel_size = (float)(max - min) / 200.f;
  const auto mouse_pos = ImGui::GetMousePos();
  const auto hovered_value = (int)((float)std::abs((int)(mouse_pos.x - (offset_.x + 29))) * pixel_size) + min;

  if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(ImVec2(offset_.x + 29, offset_.y + 14), ImVec2(offset_.x + 29 + 201, offset_.y + 14 + 10)))
    val = hovered_value;

  if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(ImVec2(offset_.x + 21, offset_.y + 12), ImVec2(offset_.x + 21 + 8, offset_.y + 12 + 10)))
    val -= step;

  if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(ImVec2(offset_.x + 22 + 200, offset_.y + 12), ImVec2(offset_.x + 22 + 200 + 15, offset_.y + 12 + 10)))
    val += step;

  val = (std::min)(val, max);
  val = (std::max)(val, min);

  renderer_->AddText(std::format("{} [{}]", name, val), ImVec2(offset_.x + 29, offset_.y - 3), ImColor(185, 185, 185));
  renderer_->AddRectangle(ImVec2(offset_.x + 29, offset_.y + 14), ImVec2(200, 10), ImColor(50, 50, 50));

  if (val)
    renderer_->AddRectangle(ImVec2(offset_.x + 29, offset_.y + 14), ImVec2((float)(val - min) / pixel_size, 10), ImColor(195, 141, 145));

  renderer_->AddOutlinedRectangle(ImVec2(offset_.x + 29, offset_.y + 14), ImVec2(200, 10), ImColor(101, 101, 101));
  renderer_->AddText("-", ImVec2(offset_.x + 22, offset_.y + 10), ImColor(185, 185, 185));
  renderer_->AddText("+", ImVec2(offset_.x + 22 + 200 + 12, offset_.y + 11), ImColor(185, 185, 185));

  if (ImGui::IsMouseHoveringRect(ImVec2(offset_.x + 29, offset_.y + 14), ImVec2(offset_.x + 29 + 200 + 1, offset_.y + 14 + 10))) {
    renderer_->AddRectangle(ImVec2(offset_.x + 29, offset_.y + 14), ImVec2((float)(hovered_value - min) / pixel_size, 10), ImColor(101, 101, 101));
    renderer_->AddText(std::to_string(hovered_value), ImVec2(mouse_pos.x + 12, mouse_pos.y - 5), ImColor(185, 185, 185));
  }

  offset_.y += 30;
}

Window::Window(Renderer* renderer, ImVec2 size, std::string text_left, std::string text_right)
  : renderer_(renderer)
  , text_left_(std::move(text_left))
  , text_right_(std::move(text_right))
  , size_(size) {}

void Window::Begin() {
  const auto mouse_pos = ImGui::GetMousePos();
  mouse_clicked_ = ImGui::IsMouseDown(ImGuiMouseButton_Left);

  if (mouse_dragging_ && !mouse_clicked_)
    mouse_dragging_ = false;

  if (mouse_dragging_ && mouse_clicked_) {
    pos_.x = mouse_pos.x - mouse_drag_.x;
    pos_.y = mouse_pos.y - mouse_drag_.y;
  }

  if (ImGui::IsMouseHoveringRect(pos_, ImVec2(pos_.x + size_.x, pos_.y + 20))) {
    mouse_dragging_ = true;
    mouse_drag_.x = mouse_pos.x - pos_.x;
    mouse_drag_.y = mouse_pos.y - pos_.y;
  }

  renderer_->AddRectangle(pos_, size_, ImColor(50, 50, 50));
  renderer_->AddRectangle(ImVec2(pos_.x, pos_.y + 20), ImVec2(size_.x, 1), ImColor(195, 141, 146));
  renderer_->AddOutlinedRectangle(pos_, size_, ImColor(0, 0, 0, 85));

  renderer_->AddText(text_left_, ImVec2(pos_.x + 5, pos_.y + 2), ImColor(180, 180, 180));

  const auto text_right_size = ImGui::CalcTextSize(text_right_.c_str());
  renderer_->AddText(text_right_, ImVec2(pos_.x + size_.x - text_right_size.x - 5, pos_.y + 2), ImColor(180, 180, 180));
}

Groupbox Window::AddGroupbox(const std::string& name, float x, float y, float width, float height) {
  const auto text_size = ImGui::CalcTextSize(name.c_str());

  renderer_->AddOutlinedRectangle(ImVec2(pos_.x + x, pos_.y + y), ImVec2(width, height), ImColor(20, 20, 20));
  renderer_->AddOutlinedRectangle(ImVec2(pos_.x + x + 1, pos_.y + y + 1), ImVec2(width - 2, height - 2), ImColor(70, 70, 70));
  renderer_->AddRectangle(ImVec2(pos_.x + x + width / 2 - text_size.x / 2 - 3, pos_.y + y), ImVec2(text_size.x + 6, 2), ImColor(44, 44, 44));
  renderer_->AddText(name, ImVec2(pos_.x + x + width / 2 - text_size.x / 2, pos_.y + y - 6), ImColor(185, 185, 185));

  return Groupbox(renderer_, ImVec2(pos_.x + x, pos_.y + y + 12), pos_.y + y, pos_.y + y + height);
}

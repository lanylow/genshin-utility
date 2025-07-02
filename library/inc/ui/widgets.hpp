#pragma once

#include <string>

#include <imgui/imgui.h>

class Renderer;

class Groupbox {
public:
  Groupbox(Renderer* renderer, ImVec2 offset, float top, float bottom);

  void AddCheckbox(const std::string& name, bool& val, bool grayed_out = false);
  void AddSlider(const std::string& name, int& val, int min, int max, int step);

private:
  Renderer* renderer_;
  ImVec2 offset_;
  float top_;
  float bottom_;
};

class Window {
public:
  Window(Renderer* renderer, ImVec2 size, std::string text_left, std::string text_right);

  void Begin();
  Groupbox AddGroupbox(const std::string& name, float x, float y, float width, float height) const;

private:
  Renderer* renderer_;
  std::string text_left_;
  std::string text_right_;
  ImVec2 size_;
  ImVec2 pos_ = ImVec2(100.f, 100.f);
  ImVec2 mouse_drag_ = ImVec2(300.f, 300.f);
  bool mouse_clicked_ = false;
  bool mouse_dragging_ = false;
};

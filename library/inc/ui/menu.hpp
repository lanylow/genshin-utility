#pragma once

#include <chrono>

#include <ui/widgets.hpp>

class Renderer;

class Menu {
public:
  Menu(Renderer* renderer);

  void Render();

private:
  void RenderMenu();
  void RenderFpsCounter();
  void UpdateFpsCounter();

  Window window_;
  Renderer* renderer_;

  std::chrono::time_point<std::chrono::high_resolution_clock> start_;
  int frames_ = 0;
  int frame_rate_ = 0;
};

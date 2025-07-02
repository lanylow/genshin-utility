#pragma once

#include <chrono>

#include <ui/widgets.hpp>

class Renderer;
struct Config;

class Menu {
public:
  Menu(Config* config, Renderer* renderer, bool is_star_rail);

  void Render();
  void ToggleMenu();

private:
  void RenderMenu();
  void RenderFpsCounter();
  void UpdateFpsCounter();

  Window window_;
  Config* config_;
  Renderer* renderer_;
  bool is_star_rail_;
  bool opened_ = false;

  std::chrono::time_point<std::chrono::high_resolution_clock> start_;
  int frames_ = 0;
  int frame_rate_ = 0;
};

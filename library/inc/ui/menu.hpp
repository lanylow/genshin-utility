#pragma once

#include <chrono>

#include <ui/config.hpp>
#include <ui/widgets.hpp>

class Renderer;

class Menu {
public:
  Menu(Renderer* renderer);

  void ReadConfig();
  void WriteConfig();
  void Render();

  const Config& GetConfig() { return config_; }

private:
  void ToggleMenu();
  void RenderMenu();
  void RenderFpsCounter();
  void UpdateFpsCounter();

  Config config_;
  ConfigFileManager config_file_manager_;
  Window window_;
  Renderer* renderer_;

  std::chrono::time_point<std::chrono::high_resolution_clock> start_;
  int frames_ = 0;
  int frame_rate_ = 0;
};

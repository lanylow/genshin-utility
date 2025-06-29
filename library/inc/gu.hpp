#pragma once

#include <config.hpp>
#include <sdk.hpp>
#include <hooks/hooks.hpp>
#include <ui/renderer.hpp>
#include <ui/menu.hpp>

class GenshinUtility {
public:
  GenshinUtility();

  void OnSetFov(float& value);

private:
  friend class Hooks;
  
  void ReadConfig();
  void WriteConfig();

  Config config_;
  ConfigFileManager config_file_manager_;
  Sdk sdk_;
  Hooks hooks_;
  Renderer renderer_;
  Menu menu_;
  bool in_battle_ = false;
};

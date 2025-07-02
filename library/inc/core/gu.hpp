#pragma once

#include <core/config.hpp>
#include <core/sdk.hpp>
#include <hooks/hooks.hpp>
#include <ui/renderer.hpp>
#include <ui/menu.hpp>

class GenshinUtility {
public:
  GenshinUtility();

  void OnSetFov(float& value) const;

private:
  friend class Hooks;
  
  void ReadConfig();
  void WriteConfig() const;

  Config config_;
  ConfigFileManager config_file_manager_;
  Sdk sdk_;
  Hooks hooks_;
  Renderer renderer_;
  Menu menu_;
  bool in_battle_ = false;
};

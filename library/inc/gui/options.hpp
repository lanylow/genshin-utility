#pragma once

namespace GenshinUtility {
  struct FOptions {
    bool menuOpened{ true };
    bool fpsCounter{ false };
    bool disableFog{ false };
    int fpsLimit{ 120 };
    int cameraFov{ 45 };
  };

  inline FOptions Options;
}
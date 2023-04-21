#pragma once

namespace unity::sdk {
  // UnityEngine.Camera
  inline utils::fn<void> set_field_of_view;

  // UnityEngine.Application
  inline utils::fn<void> set_target_frame_rate;
  inline utils::fn<void> quit;

  // UnityEngine.QualitySettings
  inline utils::fn<void> set_vsync_count;

  // UnityEngine.RenderSettings
  inline utils::fn<void> set_fog;

  void initialize();
}
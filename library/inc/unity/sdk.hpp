#pragma once

namespace unity::sdk {
  // UnityEngine.Camera
  inline unsigned long long set_field_of_view;

  // UnityEngine.Application
  inline utils::fn<void> set_target_frame_rate;

  // UnityEngine.QualitySettings
  inline utils::fn<void> set_vsync_count;

  // UnityEngine.RenderSettings
  inline utils::fn<void> set_fog;

  void initialize();
}
#pragma once

#include <utils/function.hpp>

struct Functions {
  // UnityEngine.Camera
  utils::function<void> set_field_of_view;

  // UnityEngine.Application
  utils::function<void> set_target_frame_rate;
  utils::function<void> quit;

  // UnityEngine.QualitySettings
  utils::function<void> set_vsync_count;

  // UnityEngine.RenderSettings
  utils::function<void> set_fog;

  // Only needed in Honkai: Star Rail
  // RPG.Client.BattleGamePhase
  utils::function<void> enter;
  utils::function<void> leave;
};

class Sdk {
public:
  Sdk();

  bool IsStarRail() { return is_star_rail_; }
  const Functions& GetFuncs() { return funcs_; }

private:
  void InitGenshinImpact();
  void InitStarRail();

  Functions funcs_;
  bool is_star_rail_;
};

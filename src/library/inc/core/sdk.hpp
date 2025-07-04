#pragma once

#include <utils/function.hpp>

struct Functions {
  // UnityEngine.Camera
  Function<void> set_field_of_view;

  // UnityEngine.Application
  Function<void> set_target_frame_rate;
  Function<void> quit;

  // UnityEngine.QualitySettings
  Function<void> set_vsync_count;

  // UnityEngine.RenderSettings
  Function<void> set_fog;

  // Only needed in Honkai: Star Rail
  // RPG.Client.BattleGamePhase
  Function<void> enter;
  Function<void> leave;
};

class Sdk {
public:
  Sdk();

  bool IsStarRail() const { return is_star_rail_; }
  const Functions& GetFuncs() const { return funcs_; }

private:
  void InitGenshinImpact();
  void InitStarRail();

  Functions funcs_;
  bool is_star_rail_;
};

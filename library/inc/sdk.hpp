#pragma once

#include <utils/function.hpp>

namespace sdk {
  enum class game_type {
    genshin_impact,
    star_rail
  };

  // UnityEngine.Camera
  inline utils::function<void> set_field_of_view;

  // UnityEngine.Application
  inline utils::function<void> set_target_frame_rate;
  inline utils::function<void> quit;

  // UnityEngine.QualitySettings
  inline utils::function<void> set_vsync_count;

  // UnityEngine.RenderSettings
  inline utils::function<void> set_fog;

  // Only needed in Honkai: Star Rail
  // RPG.Client.BattleGamePhase
  inline utils::function<void> enter;
  inline utils::function<void> leave;

  // UnityPlayer
  inline utils::function<unsigned long long> get_game_manager_if_exists;

  inline sdk::game_type game;

  void initialize_genshin_impact();
  void initialize_star_rail();

  bool is_genshin_impact();
}
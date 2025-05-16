#pragma once

#include <utils/function.hpp>

namespace sdk {
  struct game_t {
    enum {
      genshin_impact,
      star_rail
    };

    static bool is(int game) {
      return value == game;
    }

    static void set(int game) {
      value = game;
    }

  private:
    static inline int value;
  };
}

namespace sdk {
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
}

namespace sdk {
  void initialize_genshin_impact();
  void initialize_star_rail();
  void initialize();
}

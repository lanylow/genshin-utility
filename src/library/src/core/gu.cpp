#include <core/gu.hpp>

GenshinUtility::GenshinUtility()
  : hooks_(this)
  , menu_(&config_, &renderer_, sdk_.IsStarRail()) {
  ReadConfig();
}

void GenshinUtility::OnSetFov(float& value) const {
  const auto& funcs = sdk_.GetFuncs();

  const auto gi = [&](float val) {
    if (val == 30.f)
      funcs.set_fog(false);

    return val >= 45.f && val <= 55.f;
  };

  const auto sr = [](float val) {
    return val != 30.f && val != 1.f;
  };

  if (const auto res = sdk_.IsStarRail() ? sr(value) : gi(value); res) {
    if (!in_battle_)
      value = (float)config_.tools.camera_fov;

    funcs.set_target_frame_rate(config_.tools.enable_vsync ? -1 : config_.tools.fps_limit);
    funcs.set_vsync_count(config_.tools.enable_vsync ? 1 : 0);

    if (!sdk_.IsStarRail())
      funcs.set_fog(!config_.tools.disable_fog);
  }
}

void GenshinUtility::ReadConfig() {
  config_file_manager_.ReadConfig(config_);

  if (config_.menu.open_on_start)
    menu_.ToggleMenu();
}

void GenshinUtility::WriteConfig() const {
  config_file_manager_.WriteConfig(config_);
}

#include <hooks/hooks.hpp>

#include <hooks/veh.hpp>
#include <sdk.hpp>

Hooks::Hooks()
  : menu_(&renderer_) {
  inst_ = this;

  menu_.ReadConfig();

  if (sdk::game_t::is(sdk::game_t::genshin_impact)) {
    veh::initialize((void*)sdk::set_field_of_view, (void*)&SetFieldOfViewGi);
  }
  else {
    set_field_of_view_.Install(sdk::set_field_of_view, &SetFieldOfView);
    quit_.Install(sdk::quit, &Quit);
    enter_.Install(sdk::enter, &Enter);
    leave_.Install(sdk::leave, &Leave);
  }
}

HRESULT Hooks::Present(IDXGISwapChain* _this, UINT sync_interval, UINT flags) {
  inst_->renderer_.Render(_this, [] { inst_->menu_.Render(); });
  return inst_->present_.GetTrampoline()(_this, sync_interval, flags);
}

HRESULT Hooks::ResizeBuffers(IDXGISwapChain* _this, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT format, UINT flags) {
  inst_->renderer_.Resize();
  return inst_->resize_buffers_.GetTrampoline()(_this, buffer_count, width, height, format, flags);
}

void Hooks::SetFieldOfView(void* _this, float value) {
  std::call_once(inst_->present_flag_, [] {
    inst_->present_.InstallSwapChain(8, &Present);
    inst_->resize_buffers_.InstallSwapChain(13, &ResizeBuffers);
  });

  const auto gi = [](float val) {
    if (val == 30.f)
      sdk::set_fog(false);

    return val >= 45.f && val <= 55.f;
  };

  const auto sr = [](float val) {
    return val != 30.f && val != 1.f;
  };

  if (const auto res = sdk::game_t::is(sdk::game_t::genshin_impact) ? gi(value) : sr(value); res) {
    const auto& config = inst_->menu_.GetConfig();
    
    if (!inst_->is_in_battle_)
      value = (float)config.tools.camera_fov;

    sdk::set_target_frame_rate(config.tools.enable_vsync ? -1 : config.tools.fps_limit);
    sdk::set_vsync_count(config.tools.enable_vsync ? 1 : 0);

    if (sdk::game_t::is(sdk::game_t::genshin_impact))
      sdk::set_fog(!config.tools.disable_fog);
  }

  inst_->set_field_of_view_.GetTrampoline()(_this, value);
}

void Hooks::SetFieldOfViewGi(void* _this, float value) {
  veh::call_original(_this, value);

  std::call_once(inst_->present_flag_, [] {
    inst_->present_.InstallSwapChain(8, &Present);
    inst_->resize_buffers_.InstallSwapChain(13, &ResizeBuffers);
  });

  if (value != 45.f)
    return;

  inst_->set_field_of_view_.Install(sdk::set_field_of_view, &SetFieldOfView);
  inst_->quit_.Install(sdk::quit, &Quit);

  veh::destroy();
}

void Hooks::Quit() {
  inst_->menu_.WriteConfig();
  inst_->quit_.GetTrampoline()();
}

void Hooks::Enter(void* _this) {
  inst_->is_in_battle_ = true;
  inst_->enter_.GetTrampoline()(_this);
}

void Hooks::Leave(void* _this, void* a1) {
  inst_->is_in_battle_ = false;
  inst_->leave_.GetTrampoline()(_this, a1);
}

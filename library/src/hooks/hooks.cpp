#include <hooks/hooks.hpp>

#include <hooks/veh.hpp>
#include <gu.hpp>

Hooks::Hooks(GenshinUtility* gu)
  : gu_(gu) {
  inst_ = this;

  auto& sdk = gu_->sdk_;
  const auto& funcs = sdk.GetFuncs();

  if (sdk.IsStarRail()) {
    set_field_of_view_.Install(funcs.set_field_of_view, &SetFieldOfView);
    quit_.Install(funcs.quit, &Quit);
    enter_.Install(funcs.enter, &Enter);
    leave_.Install(funcs.leave, &Leave);
  }
  else {
    veh::initialize((void*)funcs.set_field_of_view, (void*)&SetFieldOfViewGi);
  }
}

HRESULT Hooks::Present(IDXGISwapChain* _this, UINT sync_interval, UINT flags) {
  inst_->gu_->renderer_.Render(_this, [] { inst_->gu_->menu_.Render(); });
  return inst_->present_.GetTrampoline()(_this, sync_interval, flags);
}

HRESULT Hooks::ResizeBuffers(IDXGISwapChain* _this, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT format, UINT flags) {
  inst_->gu_->renderer_.Resize();
  return inst_->resize_buffers_.GetTrampoline()(_this, buffer_count, width, height, format, flags);
}

void Hooks::SetFieldOfView(void* _this, float value) {
  std::call_once(inst_->present_flag_, [] {
    inst_->present_.InstallSwapChain(8, &Present);
    inst_->resize_buffers_.InstallSwapChain(13, &ResizeBuffers);
  });

  inst_->gu_->OnSetFov(value);
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

  const auto& funcs = inst_->gu_->sdk_.GetFuncs();
  inst_->set_field_of_view_.Install(funcs.set_field_of_view, &SetFieldOfView);
  inst_->quit_.Install(funcs.quit, &Quit);

  veh::destroy();
}

void Hooks::Quit() {
  inst_->gu_->WriteConfig();
  inst_->quit_.GetTrampoline()();
}

void Hooks::Enter(void* _this) {
  inst_->gu_->in_battle_ = true;
  inst_->enter_.GetTrampoline()(_this);
}

void Hooks::Leave(void* _this, void* a1) {
  inst_->gu_->in_battle_ = false;
  inst_->leave_.GetTrampoline()(_this, a1);
}

#include <hooks/hooks.hpp>

#include <core/gu.hpp>

namespace {

Hooks* inst = nullptr;

template <auto>
struct HookFuncProxy;

template <typename ReturnType, typename... Args, ReturnType(Hooks::*FuncPtr)(Args...)>
struct HookFuncProxy<FuncPtr> {
  static ReturnType Func(Args... args) {
    return (inst->*FuncPtr)(std::forward<Args>(args)...);
  }
};

template <auto FuncPtr>
auto Proxy = &HookFuncProxy<FuncPtr>::Func;

}

Hooks::Hooks(GenshinUtility* gu)
  : gu_(gu) {
  inst = this;

  const auto& sdk = gu_->sdk_;
  const auto& funcs = sdk.GetFuncs();

  if (sdk.IsStarRail()) {
    set_field_of_view_.Install(funcs.set_field_of_view, Proxy<&Hooks::SetFieldOfView>);
    quit_.Install(funcs.quit, Proxy<&Hooks::Quit>);
    enter_.Install(funcs.enter, Proxy<&Hooks::Enter>);
    leave_.Install(funcs.leave, Proxy<&Hooks::Leave>);
  }
  else {
    set_field_of_view_gi_.Install(funcs.set_field_of_view, Proxy<&Hooks::SetFieldOfViewGi>);
  }
}

HRESULT Hooks::Present(IDXGISwapChain* _this, UINT sync_interval, UINT flags) {
  gu_->renderer_.Render(_this, [this] { gu_->menu_.Render(); });
  return present_.CallOriginal<HRESULT>(_this, sync_interval, flags);
}

HRESULT Hooks::ResizeBuffers(IDXGISwapChain* _this, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT format, UINT flags) {
  gu_->renderer_.Resize();
  return resize_buffers_.CallOriginal<HRESULT>(_this, buffer_count, width, height, format, flags);
}

void Hooks::SetFieldOfView(void* _this, float value) {
  std::call_once(present_flag_, [this] {
    present_.InstallSwapChain(8, Proxy<&Hooks::Present>);
    resize_buffers_.InstallSwapChain(13, Proxy<&Hooks::ResizeBuffers>);
  });

  gu_->OnSetFov(value);
  set_field_of_view_.CallOriginal<void>(_this, value);
}

void Hooks::SetFieldOfViewGi(void* _this, float value) {
  set_field_of_view_gi_.CallOriginal<void>(_this, value);

  std::call_once(present_flag_, [this] {
    present_.InstallSwapChain(8, Proxy<&Hooks::Present>);
    resize_buffers_.InstallSwapChain(13, Proxy<&Hooks::ResizeBuffers>);
  });

  if (value != 45.f)
    return;

  const auto& funcs = gu_->sdk_.GetFuncs();
  set_field_of_view_.Install(funcs.set_field_of_view, Proxy<&Hooks::SetFieldOfView>);
  quit_.Install(funcs.quit, Proxy<&Hooks::Quit>);

  set_field_of_view_gi_.Remove();
}

void Hooks::Quit() {
  gu_->WriteConfig();
  quit_.CallOriginal<void>();
}

void Hooks::Enter(void* _this) {
  gu_->in_battle_ = true;
  enter_.CallOriginal<void>(_this);
}

void Hooks::Leave(void* _this, void* a1) {
  gu_->in_battle_ = false;
  leave_.CallOriginal<void>(_this, a1);
}

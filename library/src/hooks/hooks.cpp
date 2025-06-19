#include <hooks/hooks.hpp>

#include <hooks/veh.hpp>
#include <ui/renderer.hpp>
#include <ui/menu.hpp>
#include <sdk.hpp>
#include <options.hpp>

hooks::Hooks::Hooks() {
  inst_ = this;

  if (sdk::game_t::is(sdk::game_t::genshin_impact)) {
    hooks::veh::initialize((void*)sdk::set_field_of_view, (void*)&SetFieldOfViewGi);
  }
  else {
    set_field_of_view_.Install(sdk::set_field_of_view, &SetFieldOfView);
    quit_.Install(sdk::quit, &Quit);
    enter_.Install(sdk::enter, &Enter);
    leave_.Install(sdk::leave, &Leave);
  }
}

HRESULT hooks::Hooks::Present(IDXGISwapChain* _this, UINT sync_interval, UINT flags) {
  auto& storage = inst_->present_.GetStorage();
  auto& render_data = inst_->render_data_;

  utils::call_once(storage.init_flag, [&] {
    _this->GetDevice(__uuidof(ID3D11Device), (void**)&render_data.device); // NOLINT(clang-diagnostic-language-extension-token)
    render_data.device->GetImmediateContext(&render_data.context);

    auto swap_chain_desc = DXGI_SWAP_CHAIN_DESC();
    _this->GetDesc(&swap_chain_desc);

    render_data.window = swap_chain_desc.OutputWindow;
    inst_->wndproc_.SetTrampoline(SetWindowLongPtrA(render_data.window, GWLP_WNDPROC, (LONG_PTR)&WndProc));

    ui::renderer::initialize(render_data);
    options::menu.opened = options::menu.open_on_start;
  });

  utils::call_once(storage.render_target_flag, [&] {
    ID3D11Texture2D* back_buffer = nullptr;
    _this->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer); // NOLINT(clang-diagnostic-language-extension-token)
    render_data.device->CreateRenderTargetView(back_buffer, nullptr, &render_data.render_target);
    back_buffer->Release();
  });

  ui::renderer::begin();
  storage.menu.render();
  ui::renderer::end(render_data);

  return inst_->present_.GetTrampoline<decltype(&Present)>()(_this, sync_interval, flags);
}

HRESULT hooks::Hooks::ResizeBuffers(IDXGISwapChain* _this, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT format, UINT flags) {
  auto& storage = inst_->present_.GetStorage();
  auto& render_data = inst_->render_data_;

  render_data.render_target->Release();
  render_data.render_target = nullptr;
  storage.render_target_flag.reset();

  return inst_->resize_buffers_.GetTrampoline<decltype(&ResizeBuffers)>()(_this, buffer_count, width, height, format, flags);
}

LRESULT hooks::Hooks::WndProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam) {
  if (!ui::menu::handle_message(window, msg, wparam, lparam) && options::menu.opened)
    return true;

  return CallWindowProcA(inst_->wndproc_.GetTrampoline<decltype(&WndProc)>(), window, msg, wparam, lparam);
}

void hooks::Hooks::SetFieldOfView(void* _this, float value) {
  auto& storage = inst_->set_field_of_view_.GetStorage();

  utils::call_once(storage.present_flag, [] {
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
    if (!storage.is_in_battle)
      value = (float)options::tools.camera_fov;

    sdk::set_target_frame_rate(options::tools.enable_vsync ? -1 : options::tools.fps_limit);
    sdk::set_vsync_count(options::tools.enable_vsync ? 1 : 0);

    if (sdk::game_t::is(sdk::game_t::genshin_impact))
      sdk::set_fog(!options::tools.disable_fog);
  }

  inst_->set_field_of_view_.GetTrampoline<decltype(&SetFieldOfView)>()(_this, value);
}

void hooks::Hooks::SetFieldOfViewGi(void* _this, float value) {
  hooks::veh::call_original(_this, value);

  utils::call_once(inst_->set_field_of_view_.GetStorage().present_flag, [] {
    inst_->present_.InstallSwapChain(8, &Present);
    inst_->resize_buffers_.InstallSwapChain(13, &ResizeBuffers);
  });

  if (value != 45.f)
    return;

  inst_->set_field_of_view_.Install(sdk::set_field_of_view, &SetFieldOfView);
  inst_->quit_.Install(sdk::quit, &Quit);

  hooks::veh::destroy();
}

void hooks::Hooks::Quit() {
  options::save();
  inst_->quit_.GetTrampoline<decltype(&Quit)>()();
}

void hooks::Hooks::Enter(void* _this) {
  inst_->set_field_of_view_.GetStorage().is_in_battle = true;
  inst_->enter_.GetTrampoline<decltype(&Enter)>()(_this);
}

void hooks::Hooks::Leave(void* _this, void* a1) {
  inst_->set_field_of_view_.GetStorage().is_in_battle = false;
  inst_->leave_.GetTrampoline<decltype(&Leave)>()(_this, a1);
}

#include <hooks.hpp>
#include <sdk.hpp>
#include <config.hpp>
#include <ui/variables.hpp>
#include <ui/menu.hpp>
#include <ui/ui.hpp>

#include <utils/minhook.hpp>

#include <d3d11.h>
#include <intrin.h>

void hooks::initialize() {
  utils::minhook::init();

  utils::minhook::hook(sdk::set_field_of_view, &hooks::set_field_of_view::hook, &hooks::set_field_of_view::original);
  utils::minhook::hook(sdk::quit, &hooks::quit::hook, &hooks::quit::original);

  utils::minhook::enable_all();
}

#pragma warning(disable: 6387)

long __stdcall hooks::present::hook(IDXGISwapChain* swap_chain, unsigned int sync_interval, unsigned int flags) {
  utils::call_once(hooks::present::init_flag, [&]() {
    swap_chain->GetDevice(__uuidof(ID3D11Device), (void**)(&hooks::present::device));
    hooks::present::device->GetImmediateContext(&hooks::present::context);

    DXGI_SWAP_CHAIN_DESC swap_chain_desc;
    swap_chain->GetDesc(&swap_chain_desc);
    hooks::wndproc::window = swap_chain_desc.OutputWindow;
    hooks::wndproc::original = (WNDPROC)(SetWindowLongPtrA(hooks::wndproc::window, GWLP_WNDPROC, (long long)(hooks::wndproc::hook)));

    variables::menu::opened = variables::menu::open_on_start;
    ui::menu::initialize();
  });

  utils::call_once(hooks::present::render_target_flag, [&]() {
    ID3D11Texture2D* back_buffer;
    swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(&back_buffer));
    hooks::present::device->CreateRenderTargetView(back_buffer, nullptr, &hooks::present::render_target);
    back_buffer->Release();
  });

  ui::menu::handle_frame();
  ui::begin();
  ui::menu::render_menu();
  ui::menu::render_counter();
  ui::end();

  return hooks::present::original(swap_chain, sync_interval, flags);
}

#pragma warning(default: 6387)

long __stdcall hooks::resize_buffers::hook(IDXGISwapChain* swap_chain, unsigned int buffer_count, unsigned int width, unsigned int height, DXGI_FORMAT format, unsigned int flags) {
  hooks::present::render_target->Release();
  hooks::present::render_target = nullptr;
  hooks::present::render_target_flag.reset();

  return hooks::resize_buffers::original(swap_chain, buffer_count, width, height, format, flags);
}

long long __stdcall hooks::wndproc::hook(HWND hwnd, unsigned int message, unsigned long long wparam, long long lparam) {
  if (!ui::menu::handle_message(hwnd, message, wparam, lparam) && variables::menu::opened)
    return true;

  return CallWindowProcA(hooks::wndproc::original, hwnd, message, wparam, lparam);
}

bool hooks::set_field_of_view::genshin_impact(float value) {
  if (value == 30.f)
    sdk::set_fog(false);
  
  return value == 45.f;
}

bool hooks::set_field_of_view::star_rail(float value) {
  return value != 30.f;
}

void hooks::set_field_of_view::hook(void* _this, float value) {
  utils::call_once(hooks::set_field_of_view::present_flag, []() {
    utils::minhook::hook_swap_chain(8, &hooks::present::hook, &hooks::present::original);
    utils::minhook::hook_swap_chain(13, &hooks::resize_buffers::hook, &hooks::resize_buffers::original);

    utils::minhook::enable_all();
  });

  auto floored = std::floor(value);

  auto res = sdk::is_genshin_impact() ?
    hooks::set_field_of_view::genshin_impact(floored) :
    hooks::set_field_of_view::star_rail(floored);

  if (res) {
    auto ret = (unsigned long long)(_ReturnAddress());

    if (ret != sdk::ult_fov_ret)
      value = (float)(variables::tools::camera_fov);

    sdk::set_target_frame_rate(variables::tools::enable_vsync ? -1 : variables::tools::fps_limit);
    sdk::set_vsync_count(variables::tools::enable_vsync ? 1 : 0);
    sdk::set_fog(!variables::tools::disable_fog);
  }

  return hooks::set_field_of_view::original(_this, value);
}

void hooks::quit::hook() {
  config::save();

  return hooks::quit::original();
}
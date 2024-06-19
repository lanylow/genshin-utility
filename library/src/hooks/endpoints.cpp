#include <hooks/hooks.hpp>
#include <hooks/endpoints.hpp>

#include <sdk.hpp>
#include <ui/options.hpp>
#include <ui/menu.hpp>
#include <ui/ui.hpp>

#include <intrin.h>

#pragma warning(disable: 6387)

long __stdcall hooks::endpoints::present(IDXGISwapChain* swap_chain, unsigned int sync_interval, unsigned int flags) {
  utils::call_once(hooks::present.storage.init_flag, [&]() {
    swap_chain->GetDevice(__uuidof(ID3D11Device), (void**)(&hooks::present.storage.device));
    hooks::present.storage.device->GetImmediateContext(&hooks::present.storage.context);

    DXGI_SWAP_CHAIN_DESC swap_chain_desc;
    swap_chain->GetDesc(&swap_chain_desc);
    hooks::wndproc.storage.window = swap_chain_desc.OutputWindow;
    hooks::wndproc.set_trampoline(SetWindowLongPtrA(hooks::wndproc.storage.window, GWLP_WNDPROC, (long long)(hooks::endpoints::wndproc)));

    ui::options::menu::opened = ui::options::menu::open_on_start;
    ui::menu::initialize();
  });

  utils::call_once(hooks::present.storage.render_target_flag, [&]() {
    ID3D11Texture2D* back_buffer;
    swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(&back_buffer));
    hooks::present.storage.device->CreateRenderTargetView(back_buffer, nullptr, &hooks::present.storage.render_target);
    back_buffer->Release();
  });

  ui::menu::handle_frame();
  ui::begin();
  ui::menu::render_menu();
  ui::menu::render_counter();
  ui::end();

  return hooks::present.get_trampoline<decltype(&hooks::endpoints::present)>()(swap_chain, sync_interval, flags);
}

#pragma warning(default: 6387)

long __stdcall hooks::endpoints::resize_buffers(IDXGISwapChain* swap_chain, unsigned int buffer_count, unsigned int width, unsigned int height, DXGI_FORMAT format, unsigned int flags) {
  hooks::present.storage.render_target->Release();
  hooks::present.storage.render_target = nullptr;
  hooks::present.storage.render_target_flag.reset();

  return hooks::resize_buffers.get_trampoline<decltype(&hooks::endpoints::resize_buffers)>()(swap_chain, buffer_count, width, height, format, flags);
}

long long __stdcall hooks::endpoints::wndproc(HWND hwnd, unsigned int message, unsigned long long wparam, long long lparam) {
  if (!ui::menu::handle_message(hwnd, message, wparam, lparam) && ui::options::menu::opened)
    return true;

  return CallWindowProcA(hooks::wndproc.get_trampoline<decltype(&hooks::endpoints::wndproc)>(), hwnd, message, wparam, lparam);
}

void hooks::endpoints::set_field_of_view(void* _this, float value) {
  utils::call_once(hooks::set_field_of_view.storage.present_flag, []() {
    hooks::present.install_swap_chain(8, &hooks::endpoints::present);
    hooks::resize_buffers.install_swap_chain(13, &hooks::endpoints::resize_buffers);
  });

  auto genshin_impact = [](float value) -> bool {
    if (value == 30.f)
      sdk::set_fog(false);

    return value >= 45.f && value <= 55.f;
  };

  auto star_rail = [](float value) -> bool {
    return value != 30.f;
  };

  auto floored = std::floor(value);

  auto res = sdk::is_genshin_impact() ?
    genshin_impact(floored) :
    star_rail(floored);

  if (res) {
    if (!hooks::set_field_of_view.storage.is_in_battle)
      value = (float)(ui::options::tools::camera_fov);

    sdk::set_target_frame_rate(ui::options::tools::enable_vsync ? -1 : ui::options::tools::fps_limit);
    sdk::set_vsync_count(ui::options::tools::enable_vsync ? 1 : 0);

    if (sdk::is_genshin_impact())
      sdk::set_fog(!ui::options::tools::disable_fog);
    else
      *(bool*)(sdk::get_game_manager_if_exists(20) + 348) = !ui::options::tools::disable_fog;
  }

  hooks::set_field_of_view.get_trampoline<decltype(&hooks::endpoints::set_field_of_view)>()(_this, value);
}

void hooks::endpoints::quit() {
  ui::options::save();
  hooks::quit.get_trampoline<decltype(&hooks::endpoints::quit)>()();
}

void hooks::endpoints::enter(void* _this) {
  hooks::set_field_of_view.storage.is_in_battle = true;
  hooks::enter.get_trampoline<decltype(&hooks::endpoints::enter)>()(_this);
}

void hooks::endpoints::leave(void* _this, void* a1) {
  hooks::set_field_of_view.storage.is_in_battle = false;
  hooks::leave.get_trampoline<decltype(&hooks::endpoints::leave)>()(_this, a1);
}
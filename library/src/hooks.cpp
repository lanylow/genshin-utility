#include <common.hpp>
#include <hooks.hpp>

void hooks::initialize() {
  utils::mh::init();
  kiero::init(kiero::RenderType::D3D11);

  utils::mh::hook_kiero(8, &hooks::present::hook, &hooks::present::original);
  utils::mh::hook_kiero(13, &hooks::resize_buffers::hook, &hooks::resize_buffers::original);

  utils::mh::hook(unity::sdk::set_field_of_view, &hooks::set_field_of_view::hook, &hooks::set_field_of_view::original);
  utils::mh::hook(unity::sdk::quit, &hooks::quit::hook, &hooks::quit::original);

  utils::mh::enable_all();
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
  ui::gui::begin();
  ui::menu::render_menu();
  ui::menu::render_counter();
  ui::gui::end();

  return hooks::present::original(swap_chain, sync_interval, flags);
}

#pragma warning(default: 6387)

long __stdcall hooks::resize_buffers::hook(IDXGISwapChain* swap_chain, unsigned int buffer_count, unsigned int width, unsigned int height, DXGI_FORMAT format, unsigned int flags) {
  hooks::present::render_target->Release();
  hooks::present::render_target = nullptr;
  hooks::present::render_target_flag.reset();

  return hooks::resize_buffers::original(swap_chain, buffer_count, width, height, format, flags);
}

long long __stdcall hooks::wndproc::hook(HWND window, unsigned int message, unsigned long long wparam, long long lparam) {
  if (!ui::menu::handle_message(window, message, wparam, lparam) && variables::menu::opened)
    return true;

  return CallWindowProcA(hooks::wndproc::original, window, message, wparam, lparam);
}

bool hooks::set_field_of_view::genshin_impact(float value) {
  auto floored = std::floor(value);

  if (floored == 30.f)
    unity::sdk::set_fog(false);
  else if (floored == 45.f)
    return true;

  return false;
}

bool hooks::set_field_of_view::star_rail(float value) {
  auto floored = std::floor(value);

  if (floored == 30.f)
    return false;

  return true;
}

void hooks::set_field_of_view::hook(void* _this, float value) {
  auto res = unity::sdk::game == unity::sdk::game_type::genshin_impact ?
    hooks::set_field_of_view::genshin_impact(value) : 
    hooks::set_field_of_view::star_rail(value);

  if (res) {
    value = (float)(variables::tools::camera_fov);

    unity::sdk::set_target_frame_rate(variables::tools::enable_vsync ? -1 : variables::tools::fps_limit);
    unity::sdk::set_vsync_count(variables::tools::enable_vsync ? 1 : 0);
    unity::sdk::set_fog(!variables::tools::disable_fog);
  }

  return hooks::set_field_of_view::original(_this, value);
}

void hooks::quit::hook() {
  config::save();

  return hooks::quit::original();
}
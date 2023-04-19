#include <common.hpp>
#include <hooks.hpp>

void hooks::initialize() {
  MH_Initialize();
  kiero::init(kiero::RenderType::D3D11);

  MH_CreateHook((void*)(kiero::getMethodsTable()[8]), (void*)(&hooks::present::hook), (void**)(&hooks::present::original));
  MH_CreateHook((void*)(kiero::getMethodsTable()[13]), (void*)(&hooks::resize_buffers::hook), (void**)(&hooks::resize_buffers::original));
  MH_CreateHook((void*)(unity::sdk::set_field_of_view), (void*)(&hooks::set_field_of_view::hook), (void**)(&hooks::set_field_of_view::original));
  MH_CreateHook((void*)(unity::sdk::quit), (void*)(&hooks::quit::hook), (void**)(&hooks::quit::original));

  MH_EnableHook(MH_ALL_HOOKS);
}

#pragma warning(disable: 6387)

long __stdcall hooks::present::hook(IDXGISwapChain* swap_chain, unsigned int sync_interval, unsigned int flags) {
  std::call_once(hooks::present::flag, [&]() {
    swap_chain->GetDevice(__uuidof(ID3D11Device), (void**)(&hooks::present::device));
    hooks::present::device->GetImmediateContext(&hooks::present::context);

    DXGI_SWAP_CHAIN_DESC swap_chain_desc;
    swap_chain->GetDesc(&swap_chain_desc);
    hooks::wndproc::window = swap_chain_desc.OutputWindow;
    hooks::wndproc::original = (WNDPROC)(SetWindowLongPtrA(hooks::wndproc::window, GWLP_WNDPROC, (long long)(hooks::wndproc::hook)));

    variables::menu::opened = variables::menu::open_on_start;
    ui::menu::initialize();

    QueryPerformanceFrequency(&hooks::present::performance_frequency);
    QueryPerformanceCounter(&hooks::present::performance_counter);
  });

  if (initalize_render_target) {
    ID3D11Texture2D* back_buffer;
    swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(&back_buffer));
    hooks::present::device->CreateRenderTargetView(back_buffer, nullptr, &hooks::present::render_target);
    back_buffer->Release();

    initalize_render_target = false;
  }

  hooks::present::frames++;

  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  auto delta = (double)(now.QuadPart - hooks::present::performance_counter.QuadPart) / hooks::present::performance_frequency.QuadPart;

  if (delta >= 1.0) {
    ui::menu::frame_rate = hooks::present::frames;
    hooks::present::frames = 0;
    QueryPerformanceCounter(&hooks::present::performance_counter);
  }

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
  hooks::present::initalize_render_target = true;

  return hooks::resize_buffers::original(swap_chain, buffer_count, width, height, format, flags);
}

long long __stdcall hooks::wndproc::hook(HWND window, unsigned int message, unsigned long long wparam, long long lparam) {
  if (!ui::menu::handle_message(window, message, wparam, lparam) && variables::menu::opened)
    return true;

  return CallWindowProcA(hooks::wndproc::original, window, message, wparam, lparam);
}

void hooks::set_field_of_view::hook(void* _this, float value) {
  auto floored = std::floor(value);

  if (floored == 30.f) {
    unity::sdk::set_fog(false);
  }
  else if (floored == 45.f) {
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
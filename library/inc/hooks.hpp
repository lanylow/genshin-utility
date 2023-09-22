#pragma once

#include "utils/once.hpp"

#include <d3d11.h>

namespace hooks {
  void initialize();
}

namespace hooks::present {
  inline ID3D11Device* device;
  inline ID3D11DeviceContext* context;
  inline ID3D11RenderTargetView* render_target;

  inline utils::once_flag init_flag;
  inline utils::once_flag render_target_flag;

  using function_type = long(__stdcall*)(IDXGISwapChain*, unsigned int, unsigned int);
  inline function_type original;

  long __stdcall hook(IDXGISwapChain* swap_chain, unsigned int sync_interval, unsigned int flags);
}

namespace hooks::resize_buffers {
  using function_type = long(__stdcall*)(IDXGISwapChain*, unsigned int, unsigned int, unsigned int, DXGI_FORMAT, unsigned int);
  inline function_type original;

  long __stdcall hook(IDXGISwapChain* swap_chain, unsigned int buffer_count, unsigned int width, unsigned int height, DXGI_FORMAT format, unsigned int flags);
}

namespace hooks::wndproc {
  inline HWND window;

  using function_type = long long(__stdcall*)(HWND, unsigned int, unsigned long long, long long);
  inline function_type original;

  long long __stdcall hook(HWND window, unsigned int message, unsigned long long wparam, long long lparam);
}

namespace hooks::set_field_of_view {
  inline utils::once_flag present_flag;

  using function_type = void(*)(void*, float);
  inline function_type original;

  bool genshin_impact(float value);
  bool star_rail(float value);
  void hook(void* _this, float value);
}

namespace hooks::quit {
  using function_type = void(*)();
  inline function_type original;

  void hook();
}
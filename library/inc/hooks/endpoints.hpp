#pragma once

#include <d3d11.h>

namespace hooks::endpoints {
  long __stdcall present(IDXGISwapChain* swap_chain, unsigned int sync_interval, unsigned int flags);
  long __stdcall resize_buffers(IDXGISwapChain* swap_chain, unsigned int buffer_count, unsigned int width, unsigned int height, DXGI_FORMAT format, unsigned int flags);
  long long __stdcall wndproc(HWND window, unsigned int message, unsigned long long wparam, long long lparam);
  void set_field_of_view(void* _this, float value);
  void quit();
  void enter(void* _this);
  void leave(void* _this, void* a1);
}
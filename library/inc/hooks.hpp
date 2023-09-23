#pragma once

#include <utils/once.hpp>
#include <utils/hook.hpp>

#include <d3d11.h>

namespace hooks {
  struct present_storage : utils::hook_storage {
    ID3D11Device* device{ nullptr };
    ID3D11DeviceContext* context{ nullptr };
    ID3D11RenderTargetView* render_target{ nullptr };

    utils::once_flag init_flag;
    utils::once_flag render_target_flag;
  };

  struct wndproc_storage : utils::hook_storage {
    HWND window{ nullptr };
  };

  struct set_field_of_view_storage : utils::hook_storage {
    utils::once_flag present_flag;
  };
}

namespace hooks {
  void initialize();

  inline utils::hook<hooks::present_storage> present;
  inline utils::hook<> resize_buffers;
  inline utils::hook<hooks::wndproc_storage> wndproc;
  inline utils::hook<hooks::set_field_of_view_storage> set_field_of_view;
  inline utils::hook<> quit;
}

namespace hooks::endpoints {
  long __stdcall present(IDXGISwapChain* swap_chain, unsigned int sync_interval, unsigned int flags);
  long __stdcall resize_buffers(IDXGISwapChain* swap_chain, unsigned int buffer_count, unsigned int width, unsigned int height, DXGI_FORMAT format, unsigned int flags);
  long long __stdcall wndproc(HWND window, unsigned int message, unsigned long long wparam, long long lparam);
  void set_field_of_view(void* _this, float value);
  void quit();
}

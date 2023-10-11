#pragma once

#include <utils/once.hpp>
#include <hooks/hook.hpp>

#include <d3d11.h>

namespace hooks {
  struct present_storage : hooks::hook_storage {
    ID3D11Device* device{ nullptr };
    ID3D11DeviceContext* context{ nullptr };
    ID3D11RenderTargetView* render_target{ nullptr };

    utils::once_flag init_flag;
    utils::once_flag render_target_flag;
  };

  struct wndproc_storage : hooks::hook_storage {
    HWND window{ nullptr };
  };

  struct set_field_of_view_storage : hooks::hook_storage {
    bool is_in_battle{ false };

    utils::once_flag present_flag;
  };
}

namespace hooks {
  void initialize();

  inline hooks::hook<hooks::present_storage> present;
  inline hooks::hook<> resize_buffers;
  inline hooks::hook<hooks::wndproc_storage> wndproc;
  inline hooks::hook<hooks::set_field_of_view_storage> set_field_of_view;
  inline hooks::hook<> quit;
  inline hooks::hook<> enter;
  inline hooks::hook<> leave;
}

#pragma once

#include <utils/once.hpp>
#include <hooks/hook.hpp>
#include <ui/menu.hpp>

#include <d3d11.h>

namespace hooks {
  struct RenderData {
    HWND window = nullptr;
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;
    ID3D11RenderTargetView* render_target = nullptr;
  };

  struct PresentStorage : HookStorage {
    utils::once_flag init_flag;
    utils::once_flag render_target_flag;
    ui::menu menu;
  };

  struct SetFieldOfViewStorage : HookStorage {
    bool is_in_battle = false;
    utils::once_flag present_flag;
  };
}

namespace hooks {
  class Hooks {
  public:
    Hooks();

  private:
    static HRESULT Present(IDXGISwapChain* _this, UINT sync_interval, UINT flags);
    static HRESULT ResizeBuffers(IDXGISwapChain* _this, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT format, UINT flags);
    static LRESULT WndProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam);

    static void SetFieldOfView(void* _this, float value);
    static void SetFieldOfViewGi(void* _this, float value);
    static void Quit();
    static void Enter(void* _this);
    static void Leave(void* _this, void* a1);

    static inline Hooks* inst_ = nullptr;

    RenderData render_data_;

    Hook<PresentStorage> present_;
    Hook<> resize_buffers_;
    Hook<> wndproc_;
    Hook<SetFieldOfViewStorage> set_field_of_view_;
    Hook<> quit_;
    Hook<> enter_;
    Hook<> leave_;
  };
}

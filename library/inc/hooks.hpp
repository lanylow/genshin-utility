#pragma once

namespace GenshinUtility {
  class GHooks final : public GInstance<GHooks> {
  public:
    void Init() noexcept;

  private:
    static long __stdcall PresentHookHandler(IDXGISwapChain* swapChain, UInt32 syncInterval, UInt32 flags) noexcept;
    static Int64 __stdcall WndProcHandler(HWND window, UInt32 message, UInt64 wparam, Int64 lparam) noexcept;
    static void CameraSetFieldOfViewHandler(UInt64 instance, float value, UInt64 methodInfo) noexcept;

    FHook m_presentHook;
    FHook m_cameraSetFieldOfViewHook;

    HWND m_window;
    WNDPROC m_windowProcedure;
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_deviceContext;
    ID3D11RenderTargetView* m_renderTarget;
  };
}
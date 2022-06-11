#include <common.hpp>

#include <kiero/kiero.h>

extern Int64 ImGui_ImplWin32_WndProcHandler(HWND window, UInt32 message, UInt64 wparam, Int64 lparam);

namespace GenshinUtility {
  void GHooks::Init() noexcept {
    MH_Initialize();
    kiero::init(kiero::RenderType::D3D11);

    m_presentHook.Create(reinterpret_cast<void*>(kiero::getMethodsTable()[8]), reinterpret_cast<void*>(&PresentHookHandler));
    m_cameraSetFieldOfViewHook.Create(GIl2Cpp::Get()->m_setFieldOfView, reinterpret_cast<void*>(&CameraSetFieldOfViewHandler));
  }

  long __stdcall GHooks::PresentHookHandler(IDXGISwapChain* swapChain, UInt32 syncInterval, UInt32 flags) noexcept {
    GHooks* hooks = GHooks::Get();
    static auto trampoline = hooks->m_presentHook.GetTrampoline<decltype(&PresentHookHandler)>();

    static LARGE_INTEGER performanceCounter;
    static LARGE_INTEGER frequency;
    static int frames = 0;

    static const auto once = [&swapChain, &hooks](LARGE_INTEGER* performanceCounter, LARGE_INTEGER* frequency) noexcept {
      swapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&hooks->m_device));

      DXGI_SWAP_CHAIN_DESC swapChainDesc;
      ID3D11Texture2D* backBuffer;

      hooks->m_device->GetImmediateContext(&hooks->m_deviceContext);
      swapChain->GetDesc(&swapChainDesc);
      hooks->m_window = swapChainDesc.OutputWindow;
      swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
      hooks->m_device->CreateRenderTargetView(backBuffer, nullptr, &hooks->m_renderTarget);
      backBuffer->Release();

      hooks->m_windowProcedure = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(hooks->m_window, GWLP_WNDPROC, reinterpret_cast<Int64>(WndProcHandler)));

      ImGui::CreateContext();

      ImGuiIO& io = ImGui::GetIO();
      io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
      io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\tahoma.ttf)", 14.f);

      ImGui_ImplWin32_Init(hooks->m_window);
      ImGui_ImplDX11_Init(hooks->m_device, hooks->m_deviceContext);

      QueryPerformanceFrequency(frequency);
      QueryPerformanceCounter(performanceCounter);

      return true;
    } (&performanceCounter, &frequency);

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    Float64 deltaTime = static_cast<Float64>(now.QuadPart - performanceCounter.QuadPart) / frequency.QuadPart;

    if (deltaTime >= 1.0) {
      GFpsCounter::Get()->SetFrameRate(frames);
      frames = 0;
      QueryPerformanceCounter(&performanceCounter);
    }

    frames++;

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    GMenu::Get()->Render();
    GFpsCounter::Get()->Render();

    ImGui::EndFrame();
    ImGui::Render();

    hooks->m_deviceContext->OMSetRenderTargets(1, &hooks->m_renderTarget, nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return trampoline(swapChain, syncInterval, flags);
  }

  Int64 __stdcall GHooks::WndProcHandler(HWND window, UInt32 message, UInt64 wparam, Int64 lparam) noexcept {
    GHooks* hooks = GHooks::Get();

    if (message == WM_KEYDOWN && LOWORD(wparam) == VK_INSERT)
      Options.menuOpened = !Options.menuOpened;

    if (Options.menuOpened && !ImGui_ImplWin32_WndProcHandler(window, message, wparam, lparam))
      return true;

    return CallWindowProcA(hooks->m_windowProcedure, window, message, wparam, lparam);
  }

  void GHooks::CameraSetFieldOfViewHandler(UInt64 instance, float value, UInt64 methodInfo) noexcept {
    static auto trampoline = GHooks::Get()->m_cameraSetFieldOfViewHook.GetTrampoline<decltype(&CameraSetFieldOfViewHandler)>();

    if (floor(static_cast<Float64>(value)) == 45.0)
      value = static_cast<Float32>(Options.cameraFov);

    GIl2Cpp* il2cpp = GIl2Cpp::Get();

    *reinterpret_cast<std::int32_t*>(il2cpp->m_targetFrameRate) = (Options.enableVSync ? -1 : Options.fpsLimit);
    GIl2Cpp::RunCdecl<void>(il2cpp->m_setVsyncCount, (Options.enableVSync ? 1 : 0));
    GIl2Cpp::RunCdecl<void>(il2cpp->m_setFog, !Options.disableFog);

    trampoline(instance, value, methodInfo);
  }
}
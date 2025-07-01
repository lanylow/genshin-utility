#include <utils/utils.hpp>

#include <d3d11.h>

struct WindowManager {
  HWND window;
  WNDCLASSEX wnd_class;

  WindowManager(HWND window, const WNDCLASSEX& wnd_class) : window(window), wnd_class(wnd_class) {}

  ~WindowManager() {
    DestroyWindow(window);
    UnregisterClassA(wnd_class.lpszClassName, wnd_class.hInstance);
  }
};

std::array<void*, 18> utils::GetSwapChainVmt() {
  static auto vmt = [] {
    const auto wnd_class = WNDCLASSEX{
      .cbSize = sizeof(WNDCLASSEX),
      .style = CS_HREDRAW | CS_VREDRAW,
      .lpfnWndProc = DefWindowProcA,
      .cbClsExtra = 0,
      .cbWndExtra = 0,
      .hInstance = GetModuleHandleA(nullptr),
      .hIcon = nullptr,
      .hCursor = nullptr,
      .hbrBackground = nullptr,
      .lpszMenuName = nullptr,
      .lpszClassName = "GenshinUtility",
      .hIconSm = nullptr
    };

    RegisterClassExA(&wnd_class);
    const auto window = CreateWindowExA(0, wnd_class.lpszClassName, "GenshinUtilityWindow", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, nullptr, nullptr, wnd_class.hInstance, nullptr);
    const auto manager = WindowManager(window, wnd_class);

    auto feature_level = D3D_FEATURE_LEVEL();
    const auto feature_levels = std::array{D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0};

    const auto refresh_rate = DXGI_RATIONAL{
      .Numerator = 60,
      .Denominator = 1
    };

    const auto buffer_desc = DXGI_MODE_DESC{
      .Width = 100,
      .Height = 100,
      .RefreshRate = refresh_rate,
      .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
      .ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
      .Scaling = DXGI_MODE_SCALING_UNSPECIFIED
    };

    const auto sample_desc = DXGI_SAMPLE_DESC{
      .Count = 1,
      .Quality = 0
    };

    const auto swap_chain_desc = DXGI_SWAP_CHAIN_DESC{
      .BufferDesc = buffer_desc,
      .SampleDesc = sample_desc,
      .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
      .BufferCount = 1,
      .OutputWindow = window,
      .Windowed = 1,
      .SwapEffect = DXGI_SWAP_EFFECT_DISCARD,
      .Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH,
    };

    IDXGISwapChain* swap_chain;
    ID3D11Device* device;
    ID3D11DeviceContext* context;

    auto table = std::array<void*, 18>();

    if (SUCCEEDED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, feature_levels.data(), 2, D3D11_SDK_VERSION, &swap_chain_desc, &swap_chain, &device, &feature_level, &context)))
      std::copy_n(*(void***)swap_chain, table.size(), table.begin());

    swap_chain->Release();
    device->Release();
    context->Release();

    return table;
  }();

  return vmt;
}

DWORD utils::GetPageSize() {
  static auto page_size = [] {
    auto info = SYSTEM_INFO();
    GetSystemInfo(&info);
    return info.dwPageSize;
  }();

  return page_size;
}

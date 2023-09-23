#include <utils/directx.hpp>

#include <windows.h>
#include <d3d11.h>

struct window_manager {
  HWND window;
  WNDCLASSEX wnd_class;

  window_manager(HWND window, WNDCLASSEX wnd_class) : window(window), wnd_class(wnd_class) { }

  ~window_manager() {
    DestroyWindow(window);
    UnregisterClassA(wnd_class.lpszClassName, wnd_class.hInstance);
  }
};

void** utils::directx::get_swap_chain_vmt() {
  utils::call_once(utils::directx::init_flag, [&]() {
    WNDCLASSEX wnd_class;
    wnd_class.cbSize = sizeof(WNDCLASSEX);
    wnd_class.style = CS_HREDRAW | CS_VREDRAW;
    wnd_class.lpfnWndProc = DefWindowProcA;
    wnd_class.cbClsExtra = 0;
    wnd_class.cbWndExtra = 0;
    wnd_class.hInstance = GetModuleHandleA(nullptr);
    wnd_class.hIcon = nullptr;
    wnd_class.hCursor = nullptr;
    wnd_class.hbrBackground = nullptr;
    wnd_class.lpszMenuName = nullptr;
    wnd_class.lpszClassName = "GenshinUtility";
    wnd_class.hIconSm = nullptr;

    RegisterClassExA(&wnd_class);
    auto window = CreateWindowExA(0, wnd_class.lpszClassName, "GenshinUtilityWindow", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, nullptr, nullptr, wnd_class.hInstance, nullptr);
    auto d3d11 = GetModuleHandleA("d3d11.dll");
    auto manager = window_manager(window, wnd_class);

    if (!d3d11)
      return;

    auto d3d11_create_device_and_swap_chain = (decltype(&D3D11CreateDeviceAndSwapChain))(GetProcAddress(d3d11, "D3D11CreateDeviceAndSwapChain"));

    if (!d3d11_create_device_and_swap_chain)
      return;

    D3D_FEATURE_LEVEL feature_level;
    D3D_FEATURE_LEVEL feature_levels[] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };

    DXGI_RATIONAL refresh_rate;
    refresh_rate.Numerator = 60;
    refresh_rate.Denominator = 1;

    DXGI_MODE_DESC buffer_desc;
    buffer_desc.Width = 100;
    buffer_desc.Height = 100;
    buffer_desc.RefreshRate = refresh_rate;
    buffer_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    buffer_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    buffer_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    DXGI_SAMPLE_DESC sample_desc;
    sample_desc.Count = 1;
    sample_desc.Quality = 0;

    DXGI_SWAP_CHAIN_DESC swap_chain_desc;
    swap_chain_desc.BufferDesc = buffer_desc;
    swap_chain_desc.SampleDesc = sample_desc;
    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.BufferCount = 1;
    swap_chain_desc.OutputWindow = window;
    swap_chain_desc.Windowed = 1;
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    IDXGISwapChain* swap_chain;
    ID3D11Device* device;
    ID3D11DeviceContext* context;

    auto res = d3d11_create_device_and_swap_chain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, feature_levels, 2, D3D11_SDK_VERSION, &swap_chain_desc, &swap_chain, &device, &feature_level, &context);

    if (res < 0)
      return;

    auto vmt = new void*[18];
    std::memcpy(vmt, *(void***)(swap_chain), 18 * sizeof(void*));
    utils::directx::swap_chain_vmt = vmt;

    swap_chain->Release();
    device->Release();
    context->Release();
  });

  return utils::directx::swap_chain_vmt;
}

#include <thread>

#include <hooks/hooks.hpp>
#include <options.hpp>
#include <sdk.hpp>

using namespace std::chrono_literals;

void Init() {
  if (GetModuleHandleA("StarRail.exe"))
    while (!GetModuleHandleA("UnityPlayer.dll") || !GetModuleHandleA("GameAssembly.dll"))
      std::this_thread::sleep_for(10ms);

  options::load();
  sdk::initialize();
  static auto hooks = new hooks::Hooks();
}

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID) {
  DisableThreadLibraryCalls(module);

  if (reason == DLL_PROCESS_ATTACH)
    if (const auto handle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Init, nullptr, 0, nullptr))
      CloseHandle(handle);

  return TRUE;
}

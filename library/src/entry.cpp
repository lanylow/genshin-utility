#include <thread>

#include <gu.hpp>

using namespace std::chrono_literals;

void Init() {
  if (GetModuleHandleA("StarRail.exe"))
    while (!GetModuleHandleA("UnityPlayer.dll") || !GetModuleHandleA("GameAssembly.dll"))
      std::this_thread::sleep_for(10ms);

  static auto gu = GenshinUtility();
}

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID) {
  DisableThreadLibraryCalls(module);

  if (reason == DLL_PROCESS_ATTACH)
    std::thread(Init).detach();

  return TRUE;
}

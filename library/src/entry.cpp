#include <thread>

#include <hooks/hooks.hpp>
#include <options.hpp>
#include <sdk.hpp>

using namespace std::chrono_literals;

void initialize() {
  if (GetModuleHandleA("StarRail.exe"))
    while (!GetModuleHandleA("UnityPlayer.dll") || !GetModuleHandleA("GameAssembly.dll"))
      std::this_thread::sleep_for(10ms);

  options::load();
  sdk::initialize();
  hooks::initialize();
}

bool DllMain(HMODULE module, unsigned int reason, void*) {
  DisableThreadLibraryCalls(module);

  if (reason == DLL_PROCESS_ATTACH)
    if (const auto handle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)initialize, nullptr, 0, nullptr))
      CloseHandle(handle);

  return true;
}

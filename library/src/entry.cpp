#include <sdk.hpp>
#include <hooks/hooks.hpp>
#include <ui/options.hpp>

#include <thread>

using namespace std::chrono_literals;

void initialize() {
  if (GetModuleHandleA("StarRail.exe"))
    while (!GetModuleHandleA("UnityPlayer.dll") || !GetModuleHandleA("GameAssembly.dll"))
      std::this_thread::sleep_for(10ms);

  ui::options::load();
  sdk::initialize();
  hooks::initialize();
}

[[maybe_unused]] bool DllMain(HMODULE module, unsigned int reason, [[maybe_unused]] void* reserved) {
  DisableThreadLibraryCalls(module);

  if (reason == DLL_PROCESS_ATTACH)
    if (auto handle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(initialize), nullptr, 0, nullptr))
      CloseHandle(handle);

  return true;
}
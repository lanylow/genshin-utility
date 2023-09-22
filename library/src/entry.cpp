#include <common.hpp>
#include <hooks.hpp>

void initialize() {
  while (!GetModuleHandleA("UnityPlayer.dll") || !(GetModuleHandleA("UserAssembly.dll") || GetModuleHandleA("GameAssembly.dll")))
    std::this_thread::sleep_for(10ms);

  config::load();

  GetModuleHandleA("GenshinImpact.exe") != nullptr ? 
    sdk::initialize_genshin_impact() : 
    sdk::initialize_star_rail();

  hooks::initialize();
}

[[maybe_unused]] bool DllMain(HMODULE module, unsigned int reason, [[maybe_unused]] void* reserved) {
  DisableThreadLibraryCalls(module);

  if (reason == DLL_PROCESS_ATTACH)
    if (auto handle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(initialize), nullptr, 0, nullptr))
      CloseHandle(handle);

  return true;
}
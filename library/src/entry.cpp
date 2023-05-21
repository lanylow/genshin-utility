#include <common.hpp>
#include <hooks.hpp>

void initialize() {
  do {
    std::this_thread::sleep_for(10s);
  } while (!GetModuleHandleA("UnityPlayer.dll"));

  config::load();

  GetModuleHandleA("GenshinImpact.exe") != nullptr ? 
    unity::sdk::initialize_genshin_impact() : 
    unity::sdk::initialize_star_rail();

  hooks::initialize();
}

bool DllMain(HMODULE module, unsigned int reason, void* reserved) {
  DisableThreadLibraryCalls(module);

  if (reason == DLL_PROCESS_ATTACH)
    if (auto handle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(initialize), nullptr, 0, nullptr))
      CloseHandle(handle);

  return true;
}
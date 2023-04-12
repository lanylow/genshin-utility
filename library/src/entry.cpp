#include <common.hpp>
#include <hooks.hpp>

void initialize() {
  do {
    std::this_thread::sleep_for(10s);
  } while (!GetModuleHandleA("UserAssembly.dll") && !GetModuleHandleA("UnityPlayer.dll"));

  config::load();
  unity::sdk::initialize();
  hooks::initialize();
}

bool DllMain(HMODULE module, unsigned int reason, void* reserved) {
  DisableThreadLibraryCalls(module);

  switch (reason) {
  case DLL_PROCESS_ATTACH:
    if (auto handle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(initialize), nullptr, 0, nullptr))
      CloseHandle(handle);

    break;

  case DLL_PROCESS_DETACH:
    config::save();
    break;
  }

  return true;
}
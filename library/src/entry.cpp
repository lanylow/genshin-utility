#include <common.hpp>

namespace GenshinUtility {
  void __stdcall InitThread() noexcept {
    do {
      Sleep(10000);
    } while (!GetModuleHandleA("UserAssembly.dll") && !GetModuleHandleA("UnityPlayer.dll"));

    GIl2Cpp::Get()->Init();
    GHooks::Get()->Init();
  }
}

bool __stdcall DllMain(UInt64 instance, FWindows::EDllEvents event, UInt64 reserved) {
  switch (event) {
  case FWindows::EDllEvents::ProcessAttach:
    GConfig::Get()->Load();
    FWindows::NewThread(&GenshinUtility::InitThread);
    break;

  case FWindows::EDllEvents::ProcessDetach:
    GConfig::Get()->Save();
    break;
  }

  return true;
}
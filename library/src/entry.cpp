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
  if (event != FWindows::EDllEvents::ProcessAttach)
    return false;

  FWindows::NewThread(&GenshinUtility::InitThread);

  return true;
}
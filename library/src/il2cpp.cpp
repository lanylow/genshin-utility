#include <common.hpp>

namespace GenshinUtility {
  void GIl2Cpp::Init() noexcept {
    while (!(m_userAssembly = GetModuleHandleA("UserAssembly.dll")))
      Sleep(100);

    m_resolveInternalCall = GetProcAddress(reinterpret_cast<HMODULE>(m_userAssembly), "il2cpp_resolve_icall");
  }
}
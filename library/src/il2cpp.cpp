#include <common.hpp>

namespace GenshinUtility {
  void GIl2Cpp::Init() noexcept {
    while (!(m_unityPlayer = GetModuleHandleA("UnityPlayer.dll")))
      Sleep(100);
  
    m_setFieldOfView = reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(m_unityPlayer) + 0xb6b920);
    m_targetFrameRate = reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(m_unityPlayer) + 0x1b31724);
    m_setVsyncCount = reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(m_unityPlayer) + 0x1136e80);
    m_setFog = reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(m_unityPlayer) + 0x11338d0);
  }
}
#pragma once

#include <minhook/MinHook.h>

namespace GenshinUtility {
  class FHook final {
  public:
    explicit FHook() = default;

    void Create(void* target, void* hook) noexcept {
      MH_CreateHook(target, hook, &m_trampoline);
      MH_EnableHook(target);
    }

    template <typename T>
    T GetTrampoline() noexcept {
      return reinterpret_cast<T>(m_trampoline);
    }

  private:
    void* m_trampoline = nullptr;
  };
}
#pragma once

#include <minhook/MinHook.h>

#include <utils/directx.hpp>

template <typename TrampolineType = void*>
class Hook {
public:
  template <typename TargetType, typename DetourType>
  void Install(TargetType target, DetourType detour) {
    Create((void*)target, (void*)detour);
  }

  template <typename DetourType>
  void InstallSwapChain(int index, DetourType detour) {
    Create(utils::directx::get_swap_chain_vmt()[index], (void*)detour);
  }

  template <typename CastedType = TrampolineType>
  CastedType GetTrampoline() const { return (CastedType)trampoline_; }

private:
  void Create(void* target, void* detour) {
    static auto init = MH_Initialize();
    MH_CreateHook(target, detour, &trampoline_);
    MH_EnableHook(target);
  }

  void* trampoline_ = nullptr;
};

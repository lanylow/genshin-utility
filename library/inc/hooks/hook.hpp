#pragma once

#include <type_traits>

#include <minhook/MinHook.h>

#include <utils/directx.hpp>
#include <utils/function.hpp>

namespace hooks {
  struct HookStorage {
    void* address = nullptr;
    void* trampoline = nullptr;
  };

  template <typename StorageType = HookStorage>
    requires (std::is_base_of_v<HookStorage, StorageType> || std::is_same_v<HookStorage, StorageType>) && std::is_default_constructible_v<StorageType>
  class Hook {
  public:
    template <typename TargetType, typename DetourType>
      requires std::is_pointer_v<DetourType>
    void Install(TargetType target, DetourType detour) {
      storage_.address = (void*)target;
      Create((void*)detour);
    }

    template <typename DetourType>
      requires std::is_pointer_v<DetourType>
    void InstallSwapChain(int index, DetourType detour) {
      storage_.address = utils::directx::get_swap_chain_vmt()[index];
      Create((void*)detour);
    }

    template <utils::fn_convertible TrampolineType>
    TrampolineType GetTrampoline() const { return (TrampolineType)storage_.trampoline; }

    template <utils::fn_convertible TrampolineType>
    void SetTrampoline(TrampolineType value) { storage_.trampoline = (void*)value; }

    StorageType& GetStorage() { return storage_; }

  private:
    void Create(void* detour) {
      static auto init = MH_Initialize();
      MH_CreateHook(storage_.address, detour, &storage_.trampoline);
      MH_EnableHook(storage_.address);
    }

    StorageType storage_;
  };
}

#pragma once

#include <utils/directx.hpp>
#include <utils/function.hpp>

#include <minhook/MinHook.h>

#include <type_traits>

namespace hooks {
  struct hook_storage {
    void* address = nullptr;
    void* trampoline = nullptr;
  };

  template <typename storage_type = hooks::hook_storage>
    requires (std::is_base_of_v<hook_storage, storage_type> || std::is_same_v<hook_storage, storage_type>) && std::is_default_constructible_v<storage_type>
  class hook {
  public:
    template <typename target_type, typename detour_type>
      requires std::is_pointer_v<detour_type>
    void install(target_type target, detour_type detour) {
      storage.address = (void*)target;
      create((void*)detour);
    }

    template <typename detour_type>
      requires std::is_pointer_v<detour_type>
    void install_swap_chain(int index, detour_type detour) {
      storage.address = utils::directx::get_swap_chain_vmt()[index];
      create((void*)detour);
    }

    template <utils::fn_convertible trampoline_type>
    trampoline_type get_trampoline() const {
      return (trampoline_type)storage.trampoline;
    }

    template <utils::fn_convertible trampoline_type>
    void set_trampoline(trampoline_type value) {
      storage.trampoline = (void*)value;
    }

    storage_type& get_storage() {
      return storage;
    }

  private:
    void create(void* detour) {
      MH_CreateHook(storage.address, detour, &storage.trampoline);
      MH_EnableHook(storage.address);
    }

    storage_type storage;
  };
}

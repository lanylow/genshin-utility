#pragma once

#include <utils/directx.hpp>
#include <utils/function.hpp>

#include <minhook/MinHook.h>

#include <type_traits>

namespace utils {
  class hook {
  public:
    constexpr hook() = default;

  private:
    void create(void* detour) {
      MH_CreateHook(address, detour, &trampoline);
      MH_EnableHook(address);
    }

  public:
    template <typename target_type, typename detour_type,
      std::enable_if_t<std::is_pointer_v<detour_type>, int> = 0>
    constexpr void install(target_type target, detour_type detour) {
      address = (void*)(target);
      create((void*)(detour));
    }

    template <typename detour_type,
      std::enable_if_t<std::is_pointer_v<detour_type>, int> = 0>
    void install_swap_chain(int index, detour_type detour) {
      address = utils::directx::get_swap_chain_vmt()[index];
      create((void*)(detour));
    }

    template <typename trampoline_type,
      std::enable_if_t<utils::is_fn_convertible<trampoline_type>::value, int> = 0>
    constexpr trampoline_type get_trampoline() const {
      return (trampoline_type)(trampoline);
    }

    template <typename trampoline_type,
      std::enable_if_t<utils::is_fn_convertible<trampoline_type>::value, int> = 0>
    constexpr void set_trampoline(trampoline_type value) {
      trampoline = (void*)(value);
    }

  private:
    void* address{ };
    void* trampoline{ };
  };
}
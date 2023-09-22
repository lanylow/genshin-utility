#pragma once

#include "dx.hpp"

#include <minhook/MinHook.h>

#include <type_traits>

namespace utils::mh {
  inline void init() {
    MH_Initialize();
  }

  template <typename target_type, typename detour_type, typename original_type,
    std::enable_if_t<std::is_pointer_v<detour_type> && std::is_pointer_v<original_type>, int> = 0>
  inline void hook(target_type target, detour_type detour, original_type original) {
    MH_CreateHook((void*)(target), (void*)(detour), (void**)(original));
  }

  template <typename detour_type, typename original_type,
    std::enable_if_t<std::is_pointer_v<detour_type> && std::is_pointer_v<original_type>, int> = 0>
  inline void hook_swap_chain(int index, detour_type detour, original_type original) {
    MH_CreateHook(utils::dx::get_swap_chain_vmt()[index], (void*)(detour), (void**)(original));
  }

  inline void enable_all() {
    MH_EnableHook(MH_ALL_HOOKS);
  }
}
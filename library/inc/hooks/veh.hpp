#pragma once

#include <windows.h>

#include <vector>

namespace hooks::veh {
  struct veh_data_t {
    SYSTEM_INFO system_info;
    void* handle;
    void* target;
    void* detour;
  };
}

namespace hooks::veh {
  void initialize(void* target, void* detour);
  void destroy();

  template <typename... arg_type>
  void call_original(arg_type... args);

  inline veh_data_t data;
}

template <typename... arg_type>
void hooks::veh::call_original(arg_type... args) {
  unsigned long old;
  VirtualProtect(data.target, data.system_info.dwPageSize, PAGE_EXECUTE_READ, &old);
  reinterpret_cast<void(*)(arg_type...)>(data.target)(args...);
  VirtualProtect(data.target, data.system_info.dwPageSize, old, &old);
}

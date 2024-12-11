#pragma once

#include <utils/once.hpp>

namespace utils::directx {
  inline utils::once_flag init_flag;
  inline void** swap_chain_vmt;

  void** get_swap_chain_vmt();
}
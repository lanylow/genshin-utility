#pragma once

#include <utils/once.hpp>

namespace utils::directx {
  inline void** swap_chain_vmt;

  inline utils::once_flag init_flag;

  void** get_swap_chain_vmt();
}
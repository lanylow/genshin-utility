#include <sdk.hpp>

#include <windows.h>

Sdk::Sdk() {
  if (GetModuleHandleA("StarRail.exe"))
    InitStarRail();
  else
    InitGenshinImpact();
}

void Sdk::InitGenshinImpact() {
  const auto mod = (uintptr_t)GetModuleHandleA(nullptr);

  funcs_.set_field_of_view = mod + 0x1066530;
  funcs_.set_target_frame_rate = mod + 0x106aca0;
  funcs_.quit = mod + 0x106a820;
  funcs_.set_vsync_count = mod + 0x938a20;
  funcs_.set_fog = mod + 0x937a90;

  is_star_rail_ = false;
}

void Sdk::InitStarRail() {
  const auto mod = (uintptr_t)GetModuleHandleA("GameAssembly.dll");

  funcs_.set_field_of_view = mod + 0xe6e8100;
  funcs_.set_target_frame_rate = mod + 0xe6e1ff0;
  funcs_.quit = mod + 0xe6e1b20;
  funcs_.set_vsync_count = mod + 0xe7786a0;
  funcs_.enter = mod + 0x814abb0;
  funcs_.leave = mod + 0x814d860;

  is_star_rail_ = true;
}

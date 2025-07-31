#include <core/sdk.hpp>

#include <windows.h>

Sdk::Sdk() {
  if (GetModuleHandleA("StarRail.exe"))
    InitStarRail();
  else
    InitGenshinImpact();
}

void Sdk::InitGenshinImpact() {
  const auto mod = (uintptr_t)GetModuleHandleA(nullptr);

  funcs_.set_field_of_view = mod + 0x10a7c30;
  funcs_.set_target_frame_rate = mod + 0x12a1a20;
  funcs_.quit = mod + 0x10abf20;
  funcs_.set_vsync_count = mod + 0x975e20;
  funcs_.set_fog = mod + 0x974e90;

  is_star_rail_ = false;
}

void Sdk::InitStarRail() {
  const auto game_assembly = (uintptr_t)GetModuleHandleA("GameAssembly.dll");
  const auto unity_player = (uintptr_t)GetModuleHandleA("UnityPlayer.dll");

  funcs_.set_field_of_view = unity_player + 0x9d2d90;
  funcs_.set_target_frame_rate = game_assembly + 0x129ae8b0;
  funcs_.quit = game_assembly + 0x129ae3e0;
  funcs_.set_vsync_count = game_assembly + 0x129ed520;
  funcs_.enter = game_assembly + 0xcc35510;
  funcs_.leave = game_assembly + 0xcc390e0;

  is_star_rail_ = true;
}

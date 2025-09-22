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

  funcs_.set_field_of_view = mod + 0xfe0da0;
  funcs_.set_target_frame_rate = mod + 0x11db980;
  funcs_.quit = mod + 0xfe5390;
  funcs_.set_vsync_count = mod + 0x8ad650;
  funcs_.set_fog = mod + 0x8ac6c0;

  is_star_rail_ = false;
}

void Sdk::InitStarRail() {
  const auto game_assembly = (uintptr_t)GetModuleHandleA("GameAssembly.dll");
  const auto unity_player = (uintptr_t)GetModuleHandleA("UnityPlayer.dll");

  funcs_.set_field_of_view = unity_player + 0xf220e0;
  funcs_.set_target_frame_rate = game_assembly + 0x13364070;
  funcs_.quit = game_assembly + 0x13363ba0;
  funcs_.set_vsync_count = game_assembly + 0x133a33b0;
  funcs_.enter = game_assembly + 0xd654f70;
  funcs_.leave = game_assembly + 0xd658720;

  is_star_rail_ = true;
}

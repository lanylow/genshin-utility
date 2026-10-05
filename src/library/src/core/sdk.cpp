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

  funcs_.set_field_of_view = mod + 0x1454150;
  funcs_.set_target_frame_rate = mod + 0x16a5840;
  funcs_.quit = mod + 0x1459060;
  funcs_.set_vsync_count = mod + 0x97fa40;
  funcs_.set_fog = mod + 0x97ea50;

  is_star_rail_ = false;
}

void Sdk::InitStarRail() {
  const auto game_assembly = (uintptr_t)GetModuleHandleA("GameAssembly.dll");
  const auto unity_player = (uintptr_t)GetModuleHandleA("UnityPlayer.dll");

  funcs_.set_field_of_view = unity_player + 0xfa2940;
  funcs_.set_target_frame_rate = game_assembly + 0x1f3cefd0;
  funcs_.quit = game_assembly + 0x1f3ceb30;
  funcs_.set_vsync_count = game_assembly + 0x1f40f910;
  funcs_.enter = game_assembly + 0xc82d200;
  funcs_.leave = game_assembly + 0xc831a90;

  is_star_rail_ = true;
}

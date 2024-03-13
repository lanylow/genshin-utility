#include <sdk.hpp>

#include <windows.h>

void sdk::initialize_genshin_impact() {
  auto unity_player = (unsigned long long)(GetModuleHandleA("UnityPlayer.dll"));

  sdk::set_field_of_view = unity_player + 0xd14f20;

  sdk::set_target_frame_rate = unity_player + 0xaa7f00;
  sdk::quit = unity_player + 0xd0c270;

  sdk::set_vsync_count = unity_player + 0x13b2240;

  sdk::set_fog = unity_player + 0x13ad7c0;

  sdk::game = sdk::game_type::genshin_impact;
}

void sdk::initialize_star_rail() {
  auto game_assembly = (unsigned long long)(GetModuleHandleA("GameAssembly.dll"));
  auto unity_player = (unsigned long long)(GetModuleHandleA("UnityPlayer.dll"));

  sdk::set_field_of_view = game_assembly + 0x25d11d0;

  sdk::set_target_frame_rate = game_assembly + 0x28800b0;
  sdk::quit = game_assembly + 0x287eca0;

  sdk::set_vsync_count = game_assembly + 0x2819d90;

  sdk::set_fog = unity_player + 0xe6f470;

  sdk::enter = game_assembly + 0x367aef0;
  sdk::leave = game_assembly + 0x367c380;

  sdk::game = sdk::game_type::star_rail;
}

bool sdk::is_genshin_impact() {
  return sdk::game == sdk::game_type::genshin_impact;
}
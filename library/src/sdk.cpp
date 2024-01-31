#include <sdk.hpp>

#include <windows.h>

void sdk::initialize_genshin_impact() {
  auto unity_player = (unsigned long long)(GetModuleHandleA("UnityPlayer.dll"));

  sdk::set_field_of_view = unity_player + 0xd06d80;

  sdk::set_target_frame_rate = unity_player + 0xd016a0;
  sdk::quit = unity_player + 0xcfd080;

  sdk::set_vsync_count = unity_player + 0x138f5c0;

  sdk::set_fog = unity_player + 0x1391e90;

  sdk::game = sdk::game_type::genshin_impact;
}

void sdk::initialize_star_rail() {
  auto game_assembly = (unsigned long long)(GetModuleHandleA("GameAssembly.dll"));
  auto unity_player = (unsigned long long)(GetModuleHandleA("UnityPlayer.dll"));

  sdk::set_field_of_view = game_assembly + 0x6b73750;

  sdk::set_target_frame_rate = game_assembly + 0x6e1b350;
  sdk::quit = game_assembly + 0x6e19f40;

  sdk::set_vsync_count = game_assembly + 0x6dd99c0;

  sdk::set_fog = unity_player + 0xeb3210;

  sdk::enter = game_assembly + 0x18c5ef0;
  sdk::leave = game_assembly + 0x18c7590;

  sdk::game = sdk::game_type::star_rail;
}

bool sdk::is_genshin_impact() {
  return sdk::game == sdk::game_type::genshin_impact;
}
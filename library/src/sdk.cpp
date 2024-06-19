#include <sdk.hpp>

#include <windows.h>

void sdk::initialize_genshin_impact() {
  auto unity_player = (unsigned long long)(GetModuleHandleA("UnityPlayer.dll"));

  sdk::set_field_of_view = unity_player + 0xbc0530;

  sdk::set_target_frame_rate = unity_player + 0x94c530;
  sdk::quit = unity_player + 0xbb6180;

  sdk::set_vsync_count = unity_player + 0x1316fc0;

  sdk::set_fog = unity_player + 0x1318c00;

  sdk::game = sdk::game_type::genshin_impact;
}

void sdk::initialize_star_rail() {
  auto game_assembly = (unsigned long long)(GetModuleHandleA("GameAssembly.dll"));
  auto unity_player = (unsigned long long)(GetModuleHandleA("UnityPlayer.dll"));

  sdk::set_field_of_view = game_assembly + 0x676cf10;

  sdk::set_target_frame_rate = game_assembly + 0x69960c0;
  sdk::quit = game_assembly + 0x6994d40;

  sdk::set_vsync_count = game_assembly + 0x6a60f60;

  sdk::set_fog = unity_player + 0x0;

  sdk::enter = game_assembly + 0xcfd080;
  sdk::leave = game_assembly + 0x173f260;

  sdk::get_game_manager_if_exists = unity_player + 0x90cd20;

  sdk::game = sdk::game_type::star_rail;
}

bool sdk::is_genshin_impact() {
  return sdk::game == sdk::game_type::genshin_impact;
}
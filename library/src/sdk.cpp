#include <sdk.hpp>

#include <windows.h>

void sdk::initialize_genshin_impact() {
  auto unity_player = (unsigned long long)(GetModuleHandleA("UnityPlayer.dll"));

  sdk::set_field_of_view = unity_player + 0xd2dee0;

  sdk::set_target_frame_rate = unity_player + 0xab1150;
  sdk::quit = unity_player + 0xd27ef0;

  sdk::set_vsync_count = unity_player + 0x13abfc0;

  sdk::set_fog = unity_player + 0x13a77c0;

  sdk::game = sdk::game_type::genshin_impact;
}

void sdk::initialize_star_rail() {
  auto game_assembly = (unsigned long long)(GetModuleHandleA("GameAssembly.dll"));
  auto unity_player = (unsigned long long)(GetModuleHandleA("UnityPlayer.dll"));

  sdk::set_field_of_view = unity_player + 0xe18430;

  sdk::set_target_frame_rate = unity_player + 0xe08c50;
  sdk::quit = unity_player + 0xe085d0;

  sdk::set_vsync_count = unity_player + 0xe65080;

  sdk::set_fog = unity_player + 0xe6f9b0;

  sdk::enter = game_assembly + 0x3a10eb0;
  sdk::leave = game_assembly + 0x3a12630;

  sdk::game = sdk::game_type::star_rail;
}

bool sdk::is_genshin_impact() {
  return sdk::game == sdk::game_type::genshin_impact;
}
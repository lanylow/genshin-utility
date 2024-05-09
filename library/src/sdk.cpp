#include <sdk.hpp>

#include <windows.h>

void sdk::initialize_genshin_impact() {
  auto unity_player = (unsigned long long)(GetModuleHandleA("UnityPlayer.dll"));

  sdk::set_field_of_view = unity_player + 0xd2cf60;

  sdk::set_target_frame_rate = unity_player + 0xabac70;
  sdk::quit = unity_player + 0xd23020;

  sdk::set_vsync_count = unity_player + 0x147ead0;

  sdk::set_fog = unity_player + 0x14805f0;

  sdk::game = sdk::game_type::genshin_impact;
}

void sdk::initialize_star_rail() {
  auto game_assembly = (unsigned long long)(GetModuleHandleA("GameAssembly.dll"));
  auto unity_player = (unsigned long long)(GetModuleHandleA("UnityPlayer.dll"));

  sdk::set_field_of_view = game_assembly + 0x2bdab40;

  sdk::set_target_frame_rate = game_assembly + 0x2cd1530;
  sdk::quit = game_assembly + 0x2cd01b0;

  sdk::set_vsync_count = game_assembly + 0x2d86b00;

  sdk::set_fog = unity_player + 0x0;

  sdk::enter = game_assembly + 0x32bf1e0;
  sdk::leave = game_assembly + 0x32c07b0;

  sdk::get_game_manager_if_exists = unity_player + 0x905930;

  sdk::game = sdk::game_type::star_rail;
}

bool sdk::is_genshin_impact() {
  return sdk::game == sdk::game_type::genshin_impact;
}
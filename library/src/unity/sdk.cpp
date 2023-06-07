#include <common.hpp>

void unity::sdk::initialize_genshin_impact() {
  auto unity_player = (unsigned long long)(GetModuleHandleA("UnityPlayer.dll"));

  unity::sdk::set_field_of_view = unity_player + 0xc5cd70;

  unity::sdk::set_target_frame_rate = unity_player + 0xa32a20;
  unity::sdk::quit = unity_player + 0xc55cc0;

  unity::sdk::set_vsync_count = unity_player + 0x125c2a0;

  unity::sdk::set_fog = unity_player + 0x125f250;

  unity::sdk::game = unity::sdk::game_type::genshin_impact;
}

void unity::sdk::initialize_star_rail() {
  auto game_assembly = (unsigned long long)(GetModuleHandleA("GameAssembly.dll"));
  auto unity_player = (unsigned long long)(GetModuleHandleA("UnityPlayer.dll"));

  unity::sdk::set_field_of_view = unity_player + 0xd207e0;

  unity::sdk::set_target_frame_rate = unity_player + 0xd0fd10;
  unity::sdk::quit = unity_player + 0xd0f730;

  unity::sdk::set_vsync_count = unity_player + 0xd6d450;

  unity::sdk::set_fog = unity_player + 0xd77700;

  unity::sdk::ult_fov_ret = game_assembly + 0x23d47d1;

  unity::sdk::game = unity::sdk::game_type::star_rail;
}

bool unity::sdk::is_genshin_impact() {
  return unity::sdk::game == unity::sdk::game_type::genshin_impact;
}
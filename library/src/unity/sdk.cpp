#include <common.hpp>

void unity::sdk::initialize_genshin_impact() {
  auto unity_player = (unsigned long long)(GetModuleHandleA("UnityPlayer.dll"));

  unity::sdk::set_field_of_view = unity_player + 0xc64920;

  unity::sdk::set_target_frame_rate = unity_player + 0xc600a0;
  unity::sdk::quit = unity_player + 0xc5c9e0;

  unity::sdk::set_vsync_count = unity_player + 0x1269330;

  unity::sdk::set_fog = unity_player + 0x1264520;

  unity::sdk::game = unity::sdk::game_type::genshin_impact;
}

void unity::sdk::initialize_star_rail() {
  auto game_assembly = (unsigned long long)(GetModuleHandleA("GameAssembly.dll"));
  auto unity_player = (unsigned long long)(GetModuleHandleA("UnityPlayer.dll"));

  unity::sdk::set_field_of_view = unity_player + 0xd28c20;

  unity::sdk::set_target_frame_rate = unity_player + 0xd18100;
  unity::sdk::quit = unity_player + 0xd17b20;

  unity::sdk::set_vsync_count = unity_player + 0xd75c50;

  unity::sdk::set_fog = unity_player + 0xd80af0;

  unity::sdk::ult_fov_ret = game_assembly + 0x2c43a21;

  unity::sdk::game = unity::sdk::game_type::star_rail;
}

bool unity::sdk::is_genshin_impact() {
  return unity::sdk::game == unity::sdk::game_type::genshin_impact;
}
#include <sdk.hpp>

#include <windows.h>

void sdk::initialize_genshin_impact_global() {
  auto mod = (unsigned long long)(GetModuleHandleA(nullptr));

  sdk::set_field_of_view = mod + 0xf08370;

  sdk::set_target_frame_rate = mod + 0x110c970;
  sdk::quit = mod + 0xf0c720;

  sdk::set_vsync_count = mod + 0x7b5470;

  sdk::set_fog = mod + 0x7b4520;

  sdk::game_t::set(sdk::game_t::genshin_impact);
}

void sdk::initialize_genshin_impact_chinese() {
  auto mod = (unsigned long long)(GetModuleHandleA(nullptr));

  sdk::set_field_of_view = mod + 0xf0a370;

  sdk::set_target_frame_rate = mod + 0x110e970;
  sdk::quit = mod + 0xf0e720;

  sdk::set_vsync_count = mod + 0x7b7470;

  sdk::set_fog = mod + 0x7b6520;

  sdk::game_t::set(sdk::game_t::genshin_impact);
}

void sdk::initialize_star_rail() {
  auto game_assembly = (unsigned long long)(GetModuleHandleA("GameAssembly.dll"));

  sdk::set_field_of_view = game_assembly + 0x682b70;

  sdk::set_target_frame_rate = game_assembly + 0x7cdc80;
  sdk::quit = game_assembly + 0x7cc8f0;

  sdk::set_vsync_count = game_assembly + 0x83c360;

  sdk::enter = game_assembly + 0x17a8760;
  sdk::leave = game_assembly + 0x17a99c0;

  sdk::game_t::set(sdk::game_t::star_rail);
}

void sdk::initialize() {
  if (GetModuleHandleA("StarRail.exe"))
    sdk::initialize_star_rail();
  else if (GetModuleHandleA("GenshinImpact.exe"))
    sdk::initialize_genshin_impact_global();
  else
    sdk::initialize_genshin_impact_chinese();
}

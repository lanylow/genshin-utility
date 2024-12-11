#include <sdk.hpp>

#include <windows.h>

void sdk::initialize_genshin_impact_global() {
  auto mod = (unsigned long long)(GetModuleHandleA(nullptr));

  sdk::set_field_of_view = mod + 0x13f87c0;

  sdk::set_target_frame_rate = mod + 0x15fcc80;
  sdk::quit = mod + 0x13fcc80;

  sdk::set_vsync_count = mod + 0xca9ca0;

  sdk::set_fog = mod + 0xca8d50;

  sdk::game_t::set(sdk::game_t::genshin_impact);
}

void sdk::initialize_genshin_impact_chinese() {
  auto mod = (unsigned long long)(GetModuleHandleA(nullptr));

  sdk::set_field_of_view = mod + 0x13f38a0;

  sdk::set_target_frame_rate = mod + 0x15f7d60;
  sdk::quit = mod + 0x13f7d60;

  sdk::set_vsync_count = mod + 0xca4da0;

  sdk::set_fog = mod + 0xca3e50;

  sdk::game_t::set(sdk::game_t::genshin_impact);
}

void sdk::initialize_star_rail() {
  auto game_assembly = (unsigned long long)(GetModuleHandleA("GameAssembly.dll"));

  sdk::set_field_of_view = game_assembly + 0x1889690;

  sdk::set_target_frame_rate = game_assembly + 0x19d2c40;
  sdk::quit = game_assembly + 0x19d18b0;

  sdk::set_vsync_count = game_assembly + 0x1a31a00;

  sdk::enter = game_assembly + 0x2904390;
  sdk::leave = game_assembly + 0x2905600;

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

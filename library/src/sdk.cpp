#include <sdk.hpp>

#include <windows.h>

void sdk::initialize_genshin_impact_global() {
  auto mod = (unsigned long long)(GetModuleHandleA(nullptr));

  sdk::set_field_of_view = mod + 0x165a1d0;

  sdk::set_target_frame_rate = mod + 0x185be40;
  sdk::quit = mod + 0x165e590;

  sdk::set_vsync_count = mod + 0xf79000;

  sdk::set_fog = mod + 0xf780b0;

  sdk::game_t::set(sdk::game_t::genshin_impact);
}

void sdk::initialize_genshin_impact_chinese() {
  auto mod = (unsigned long long)(GetModuleHandleA(nullptr));

  sdk::set_field_of_view = mod + 0x165f1d0;

  sdk::set_target_frame_rate = mod + 0x1860e40;
  sdk::quit = mod + 0x1663590;

  sdk::set_vsync_count = mod + 0xf7e000;

  sdk::set_fog = mod + 0xf7d0b0;

  sdk::game_t::set(sdk::game_t::genshin_impact);
}

void sdk::initialize_star_rail() {
  auto game_assembly = (unsigned long long)(GetModuleHandleA("GameAssembly.dll"));

  sdk::set_field_of_view = game_assembly + 0x65b8110;

  sdk::set_target_frame_rate = game_assembly + 0x6870da0;
  sdk::quit = game_assembly + 0x68708a0;

  sdk::set_vsync_count = game_assembly + 0x696dc50;

  sdk::enter = game_assembly + 0x7609fc0;
  sdk::leave = game_assembly + 0x760cb50;

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

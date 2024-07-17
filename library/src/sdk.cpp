#include <sdk.hpp>

#include <windows.h>

void sdk::initialize_genshin_impact_global() {
  auto mod = (unsigned long long)(GetModuleHandleA(nullptr));

  sdk::set_field_of_view = mod + 0x1688e60;

  sdk::set_target_frame_rate = mod + 0x18834d0;
  sdk::quit = mod + 0x168cf60;

  sdk::set_vsync_count = mod + 0xfb39f0;

  sdk::set_fog = mod + 0xfb2ad0;

  sdk::game_t::set(sdk::game_t::genshin_impact);
}

void sdk::initialize_genshin_impact_chinese() {
  auto mod = (unsigned long long)(GetModuleHandleA(nullptr));

  sdk::set_field_of_view = mod + 0x1684560;

  sdk::set_target_frame_rate = mod + 0x187ebd0;
  sdk::quit = mod + 0x1688660;

  sdk::set_vsync_count = mod + 0xfaf0f0;

  sdk::set_fog = mod + 0xfae1d0;

  sdk::game_t::set(sdk::game_t::genshin_impact);
}

void sdk::initialize_star_rail() {
  auto game_assembly = (unsigned long long)(GetModuleHandleA("GameAssembly.dll"));

  sdk::set_field_of_view = game_assembly + 0x676cf10;

  sdk::set_target_frame_rate = game_assembly + 0x69960c0;
  sdk::quit = game_assembly + 0x6994d40;

  sdk::set_vsync_count = game_assembly + 0x6a60f60;

  sdk::enter = game_assembly + 0xcfd080;
  sdk::leave = game_assembly + 0x173f260;

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

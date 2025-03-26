#include <sdk.hpp>

#include <windows.h>

void sdk::initialize_genshin_impact_global() {
  auto mod = (unsigned long long)(GetModuleHandleA(nullptr));

  sdk::set_field_of_view = mod + 0xfa2f70;

  sdk::set_target_frame_rate = mod + 0xfa76b0;
  sdk::quit = mod + 0xfa7230;

  sdk::set_vsync_count = mod + 0x885e70;

  sdk::set_fog = mod + 0x884ee0;

  sdk::game_t::set(sdk::game_t::genshin_impact);
}

void sdk::initialize_genshin_impact_chinese() {
  auto mod = (unsigned long long)(GetModuleHandleA(nullptr));

  sdk::set_field_of_view = mod + 0xfa0f70;

  sdk::set_target_frame_rate = mod + 0xfa56b0;
  sdk::quit = mod + 0xfa5230;

  sdk::set_vsync_count = mod + 0x883e70;

  sdk::set_fog = mod + 0x882ee0;

  sdk::game_t::set(sdk::game_t::genshin_impact);
}

void sdk::initialize_star_rail() {
  auto game_assembly = (unsigned long long)(GetModuleHandleA("GameAssembly.dll"));

  sdk::set_field_of_view = game_assembly + 0x6f9a870;

  sdk::set_target_frame_rate = game_assembly + 0x72c6ab0;
  sdk::quit = game_assembly + 0x72c65e0;

  sdk::set_vsync_count = game_assembly + 0x73746c0;

  sdk::enter = game_assembly + 0x8570780;
  sdk::leave = game_assembly + 0x8573110;

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

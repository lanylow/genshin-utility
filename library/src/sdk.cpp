#include <sdk.hpp>

#include <windows.h>

void sdk::initialize_genshin_impact() {
  const auto mod = (uintptr_t)GetModuleHandleA(nullptr);

  sdk::set_field_of_view = mod + 0x1066530;

  sdk::set_target_frame_rate = mod + 0x106aca0;
  sdk::quit = mod + 0x106a820;

  sdk::set_vsync_count = mod + 0x938a20;

  sdk::set_fog = mod + 0x937a90;

  sdk::game_t::set(sdk::game_t::genshin_impact);
}

void sdk::initialize_star_rail() {
  const auto game_assembly = (uintptr_t)GetModuleHandleA("GameAssembly.dll");

  sdk::set_field_of_view = game_assembly + 0xe6e8100;

  sdk::set_target_frame_rate = game_assembly + 0xe6e1ff0;
  sdk::quit = game_assembly + 0xe6e1b20;

  sdk::set_vsync_count = game_assembly + 0xe7786a0;

  sdk::enter = game_assembly + 0x814abb0;
  sdk::leave = game_assembly + 0x814d860;

  sdk::game_t::set(sdk::game_t::star_rail);
}

void sdk::initialize() {
  if (GetModuleHandleA("StarRail.exe"))
    sdk::initialize_star_rail();
  else
    sdk::initialize_genshin_impact();
}

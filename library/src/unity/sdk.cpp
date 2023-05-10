#include <common.hpp>

void unity::sdk::initialize_genshin_impact() {
  auto unity_player = (unsigned long long)(GetModuleHandleA("UnityPlayer.dll"));

  unity::sdk::set_field_of_view = unity_player + 0xc43a20;

  unity::sdk::set_target_frame_rate = unity_player + 0xa2a0f0;
  unity::sdk::quit = unity_player + 0xc3d060;

  unity::sdk::set_vsync_count = unity_player + 0x1245fe0;

  unity::sdk::set_fog = unity_player + 0x1240e70;

  unity::sdk::game = unity::sdk::game_type::genshin_impact;
}

#pragma warning(disable: 6387)

void unity::sdk::initialize_star_rail() {
  auto game_assembly = GetModuleHandleA("GameAssembly.dll");
  auto resolve_icall = utils::fn<void*>{ GetProcAddress(game_assembly, "il2cpp_resolve_icall") };

  unity::sdk::set_field_of_view = resolve_icall("UnityEngine.Camera::set_fieldOfView(System.Single)");

  unity::sdk::set_target_frame_rate = resolve_icall("UnityEngine.Application::set_targetFrameRate(System.Int32)");
  unity::sdk::quit = resolve_icall("UnityEngine.Application::Quit(System.Int32)");

  unity::sdk::set_vsync_count = resolve_icall("UnityEngine.QualitySettings::set_vSyncCount(System.Int32)");

  unity::sdk::set_fog = resolve_icall("UnityEngine.RenderSettings::set_fog(System.Boolean)");

  unity::sdk::game = unity::sdk::game_type::star_rail;
}

#pragma warning(default: 6387)
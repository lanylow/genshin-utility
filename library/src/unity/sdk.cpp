#include <common.hpp>

void unity::sdk::initialize() {
  auto unity_player = (unsigned long long)(GetModuleHandleA("UnityPlayer.dll"));

  unity::sdk::quit = unity_player + 0xc3d060;

  unity::sdk::set_field_of_view = unity_player + 0xc43a20;

  unity::sdk::set_target_frame_rate = utils::fn<void>(unity_player + 0xa2a0f0);

  unity::sdk::set_vsync_count = utils::fn<void>(unity_player + 0x1245fe0);

  unity::sdk::set_fog = utils::fn<void>(unity_player + 0x1240e70);
}
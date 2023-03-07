#include <common.hpp>

void unity::sdk::initialize() {
  auto unity_player = (unsigned long long)(GetModuleHandleA("UnityPlayer.dll"));

  unity::sdk::set_field_of_view = unity_player + 0xc39840;

  unity::sdk::set_target_frame_rate = utils::fn<void>(unity_player + 0xc36b50);

  unity::sdk::set_vsync_count = utils::fn<void>(unity_player + 0x12352f0);

  unity::sdk::set_fog = utils::fn<void>(unity_player + 0x12309C0);
}
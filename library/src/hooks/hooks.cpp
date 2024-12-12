#include <hooks/hooks.hpp>
#include <hooks/endpoints.hpp>
#include <hooks/veh.hpp>
#include <sdk.hpp>

void hooks::initialize() {
  MH_Initialize();

  if (sdk::game_t::is(sdk::game_t::genshin_impact)) {
    hooks::veh::initialize((void*)sdk::set_field_of_view, (void*)&hooks::endpoints::set_field_of_view_gi);
  }
  else {
    hooks::set_field_of_view.install(sdk::set_field_of_view, &hooks::endpoints::set_field_of_view);
    hooks::quit.install(sdk::quit, &hooks::endpoints::quit);

    hooks::enter.install(sdk::enter, &hooks::endpoints::enter);
    hooks::leave.install(sdk::leave, &hooks::endpoints::leave);
  }
}

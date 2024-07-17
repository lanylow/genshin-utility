#include <hooks/hooks.hpp>
#include <hooks/endpoints.hpp>
#include <sdk.hpp>

void hooks::initialize() {
  MH_Initialize();

  hooks::set_field_of_view.install(sdk::set_field_of_view, &hooks::endpoints::set_field_of_view);
  hooks::quit.install(sdk::quit, &hooks::endpoints::quit);

  if (!sdk::game_t::is(sdk::game_t::star_rail))
    return;

  hooks::enter.install(sdk::enter, &hooks::endpoints::enter);
  hooks::leave.install(sdk::leave, &hooks::endpoints::leave);
}

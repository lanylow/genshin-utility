#include <hooks/hooks.hpp>
#include <hooks/endpoints.hpp>
#include <sdk.hpp>

void hooks::initialize() {
  MH_Initialize();

  hooks::set_field_of_view.install(sdk::set_field_of_view,  &hooks::endpoints::set_field_of_view);
  hooks::quit.install(sdk::quit, &hooks::endpoints::quit);
}

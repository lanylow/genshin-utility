#include <ui/renderer.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

#include <hooks/hooks.hpp>

void ui::renderer::initialize() {
  ImGui::CreateContext();

  auto io = ImGui::GetIO();
  io.IniFilename = nullptr;
  io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
  io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\tahoma.ttf)", 14.f);

  ImGui_ImplWin32_Init(hooks::wndproc.get_storage().window);

  const auto& storage = hooks::present.get_storage();
  ImGui_ImplDX11_Init(storage.device, storage.context);
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

long long ui::renderer::handle_message(HWND window, unsigned int message, unsigned long long wparam, long long lparam) {
  return ImGui_ImplWin32_WndProcHandler(window, message, wparam, lparam);
}

void ui::renderer::begin() {
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();
}

void ui::renderer::end() {
  ImGui::EndFrame();
  ImGui::Render();

  const auto& storage = hooks::present.get_storage();
  storage.context->OMSetRenderTargets(1, &storage.render_target, nullptr);
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ui::renderer::add_text(const char* name, ImVec2 pos, ImColor color) {
  const auto draw_list = ImGui::GetBackgroundDrawList();

  draw_list->AddText({pos.x + 1, pos.y + 1}, IM_COL32_BLACK, name);
  draw_list->AddText({pos.x - 1, pos.y - 1}, IM_COL32_BLACK, name);
  draw_list->AddText({pos.x + 1, pos.y - 1}, IM_COL32_BLACK, name);
  draw_list->AddText({pos.x - 1, pos.y + 1}, IM_COL32_BLACK, name);

  draw_list->AddText({pos.x + 1, pos.y}, IM_COL32_BLACK, name);
  draw_list->AddText({pos.x - 1, pos.y}, IM_COL32_BLACK, name);
  draw_list->AddText({pos.x, pos.y - 1}, IM_COL32_BLACK, name);
  draw_list->AddText({pos.x, pos.y + 1}, IM_COL32_BLACK, name);

  draw_list->AddText({pos.x, pos.y}, color, name);
}

void ui::renderer::add_rectangle(ImVec2 pos, ImVec2 size, ImColor color) {
  ImGui::GetBackgroundDrawList()->AddRectFilled(pos, {pos.x + size.x, pos.y + size.y}, color);
}

void ui::renderer::add_outlined_rectangle(ImVec2 pos, ImVec2 size, ImColor color) {
  ImGui::GetBackgroundDrawList()->AddRect(pos, {pos.x + size.x, pos.y + size.y}, color);
}

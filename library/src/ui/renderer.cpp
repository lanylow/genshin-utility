#include <ui/renderer.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

#include <hooks/hooks.hpp>

void ui::renderer::initialize(const hooks::RenderData& render_data) {
  ImGui::CreateContext();

  auto io = ImGui::GetIO();
  io.IniFilename = nullptr;
  io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
  io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\tahoma.ttf)", 14.f);

  ImGui_ImplWin32_Init(render_data.window);
  ImGui_ImplDX11_Init(render_data.device, render_data.context);
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT ui::renderer::handle_message(HWND window, UINT msg, WPARAM wparam, LPARAM lparam) {
  return ImGui_ImplWin32_WndProcHandler(window, msg, wparam, lparam);
}

void ui::renderer::begin() {
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();
}

void ui::renderer::end(const hooks::RenderData& render_data) {
  ImGui::EndFrame();
  ImGui::Render();

  render_data.context->OMSetRenderTargets(1, &render_data.render_target, nullptr);
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

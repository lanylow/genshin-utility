#include <ui/ui.hpp>
#include <hooks/hooks.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

#include <format>
#include <string>
#include <utility>

void ui::renderer::initialize() {
  ImGui::CreateContext();

  auto io = ImGui::GetIO();
  io.IniFilename = nullptr;
  io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
  io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\tahoma.ttf)", 14.f);

  ImGui_ImplWin32_Init(hooks::wndproc.storage.window);
  ImGui_ImplDX11_Init(hooks::present.storage.device, hooks::present.storage.context);
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

  hooks::present.storage.context->OMSetRenderTargets(1, &hooks::present.storage.render_target, nullptr);
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ui::renderer::add_text(const char* name, ImVec2 pos, int r, int g, int b, int a) {
  auto draw_list = ImGui::GetBackgroundDrawList();

  draw_list->AddText({ pos.x + 1, pos.y + 1 }, ImColor{ 0, 0, 0, 255 }, name);
  draw_list->AddText({ pos.x - 1, pos.y - 1 }, ImColor{ 0, 0, 0, 255 }, name);
  draw_list->AddText({ pos.x + 1, pos.y - 1 }, ImColor{ 0, 0, 0, 255 }, name);
  draw_list->AddText({ pos.x - 1, pos.y + 1 }, ImColor{ 0, 0, 0, 255 }, name);

  draw_list->AddText({ pos.x + 1, pos.y }, ImColor{ 0, 0, 0, 255 }, name);
  draw_list->AddText({ pos.x - 1, pos.y }, ImColor{ 0, 0, 0, 255 }, name);
  draw_list->AddText({ pos.x, pos.y - 1 }, ImColor{ 0, 0, 0, 255 }, name);
  draw_list->AddText({ pos.x, pos.y + 1 }, ImColor{ 0, 0, 0, 255 }, name);

  draw_list->AddText({ pos.x, pos.y }, ImColor{ r, g, b, a }, name);
}

void ui::renderer::add_rectangle(ImVec2 pos, ImVec2 size, int r, int g, int b, int a) {
  ImGui::GetBackgroundDrawList()->AddRectFilled(pos, { pos.x + size.x, pos.y + size.y }, ImColor{ r, g, b, a });
}

void ui::renderer::add_outlined_rectangle(ImVec2 pos, ImVec2 size, int r, int g, int b, int a) {
  ImGui::GetBackgroundDrawList()->AddRect(pos, { pos.x + size.x, pos.y + size.y }, ImColor{ r, g, b, a });
}

ui::window::window(ImVec2 size, std::string text_left, std::string text_right)
  : size(size), text_left(std::move(text_left)), text_right(std::move(text_right)) { }

void ui::window::begin() {
  mouse_clicked = ImGui::IsMouseDown(ImGuiMouseButton_Left);
  auto mouse_pos = ImGui::GetMousePos();

  if (mouse_dragging && !mouse_clicked)
    mouse_dragging = false;

  if (mouse_dragging && mouse_clicked) {
    pos.x = mouse_pos.x - mouse_drag.x;
    pos.y = mouse_pos.y - mouse_drag.y;
  }

  if (ImGui::IsMouseHoveringRect(pos, { pos.x + size.x, pos.y + 20 })) {
    mouse_dragging = true;
    mouse_drag.x = mouse_pos.x - pos.x;
    mouse_drag.y = mouse_pos.y - pos.y;
  }

  renderer::add_rectangle(pos, size, 50, 50, 50, 255);
  renderer::add_rectangle({ pos.x, pos.y + 20 }, { size.x, 1 }, 195, 141, 146, 255);
  renderer::add_outlined_rectangle(pos, size, 0, 0, 0, 85);

  renderer::add_text(text_left.c_str(), { pos.x + 5, pos.y + 2 }, 180, 180, 180, 255);

  auto text_right_size = ImGui::CalcTextSize(text_right.c_str());
  renderer::add_text(text_right.c_str(), { pos.x + size.x - text_right_size.x - 5, pos.y + 2 }, 180, 180, 180, 255);
}

void ui::window::add_groupbox(const char* name, float x, float y, float width, float height) {
  auto text_size = ImGui::CalcTextSize(name);

  renderer::add_outlined_rectangle({ pos.x + x, pos.y + y }, { width, height }, 20, 20, 20, 255);
  renderer::add_outlined_rectangle({ pos.x + x + 1, pos.y + y + 1 }, { width - 2, height - 2 }, 70, 70, 70, 255);
  renderer::add_rectangle({ pos.x + x + width / 2 - text_size.x / 2 - 3, pos.y + y }, { text_size.x + 6 , 2 }, 44, 44, 44, 255);
  renderer::add_text(name, { pos.x + x + width / 2 - text_size.x / 2, pos.y + y - 6 }, 185, 185, 185, 255);

  groupbox_offset.y = pos.y + y + 12;
  groupbox_offset.x = pos.x + x;

  groupbox_bottom = pos.y + y + height;
  groupbox_top = pos.y + y;
}

void ui::window::add_checkbox(const char* name, bool* item, bool grayed_out) {
  if (groupbox_bottom <= groupbox_offset.y + 18)
    return;

  if (groupbox_top >= groupbox_offset.y + 18)
    return;

  auto text_size = ImGui::CalcTextSize(name);

  if (!grayed_out && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect({ groupbox_offset.x + 7, groupbox_offset.y }, { groupbox_offset.x + 7 + 12 + text_size.x, groupbox_offset.y + 12 }))
    *item = !*item;

  if (grayed_out) {
    renderer::add_rectangle({ groupbox_offset.x + 8, groupbox_offset.y }, { 12, 12 }, 101, 101, 101, 255);
    renderer::add_text(name, { groupbox_offset.x + 29, groupbox_offset.y - 2 }, 101, 101, 101, 255);
  }
  else {
    if (*item)
      renderer::add_rectangle({ groupbox_offset.x + 8, groupbox_offset.y }, { 12, 12 }, 195, 141, 145, 255);
    else
      renderer::add_rectangle({ groupbox_offset.x + 8, groupbox_offset.y }, { 12, 12 }, 50, 50, 50, 255);

    renderer::add_outlined_rectangle({ groupbox_offset.x + 8, groupbox_offset.y }, { 12, 12 }, 101, 101, 101, 255);
    renderer::add_text(name, { groupbox_offset.x + 29, groupbox_offset.y - 2 }, 185, 185, 185, 255);

    if (ImGui::IsMouseHoveringRect({ groupbox_offset.x + 7, groupbox_offset.y }, { groupbox_offset.x + 7 + 12 + text_size.x, groupbox_offset.y + 12 }))
      renderer::add_outlined_rectangle({ groupbox_offset.x + 9, groupbox_offset.y + 1 }, { 10, 10 }, 101, 101, 101, 255);
  }

  groupbox_offset.y += 18;
}

void ui::window::add_slider(const char* name, int min, int max, int* item, int step) {
  if (groupbox_bottom <= groupbox_offset.y + 30)
    return;

  auto pixel_size = (float)(max - min) / 200.f;
  auto mouse_pos = ImGui::GetMousePos();
  auto hovered_value = (int)((float)(std::abs((int)(mouse_pos.x - (groupbox_offset.x + 29)))) * pixel_size) + min;

  if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect({ groupbox_offset.x + 29, groupbox_offset.y + 14 }, { groupbox_offset.x + 29 + 201, groupbox_offset.y + 14 + 10 }))
    *item = hovered_value;

  if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect({ groupbox_offset.x + 21, groupbox_offset.y + 12 }, { groupbox_offset.x + 21 + 8, groupbox_offset.y + 12 + 10 }))
    *item -= step;

  if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect({ groupbox_offset.x + 22 + 200, groupbox_offset.y + 12 }, { groupbox_offset.x + 22 + 200 + 15, groupbox_offset.y + 12 + 10 }))
    *item += step;

  if (*item > max)
    *item = max;

  if (*item < min)
    *item = min;

  auto text = std::format("{} [{}]", name, *item);
  renderer::add_text(text.c_str(), { groupbox_offset.x + 29, groupbox_offset.y - 3 }, 185, 185, 185, 255);
  renderer::add_rectangle({ groupbox_offset.x + 29, groupbox_offset.y + 14 }, { 200, 10 }, 50, 50, 50, 255);

  if (*item)
    renderer::add_rectangle({ groupbox_offset.x + 29, groupbox_offset.y + 14 }, { (float)(*item - min) / pixel_size, 10 }, 195, 141, 145, 255);

  renderer::add_outlined_rectangle({ groupbox_offset.x + 29, groupbox_offset.y + 14 }, { 200, 10 }, 101, 101, 101, 255);
  renderer::add_text("-", { groupbox_offset.x + 22, groupbox_offset.y + 10 }, 185, 185, 185, 255);
  renderer::add_text("+", { groupbox_offset.x + 22 + 200 + 12, groupbox_offset.y + 11 }, 185, 185, 185, 255);

  if (ImGui::IsMouseHoveringRect({ groupbox_offset.x + 29, groupbox_offset.y + 14 }, { groupbox_offset.x + 29 + 200 + 1, groupbox_offset.y + 14 + 10 })) {
    renderer::add_rectangle({ groupbox_offset.x + 29, groupbox_offset.y + 14 }, { (float)(hovered_value - min) / pixel_size, 10 }, 101, 101, 101, 255);
    renderer::add_text(std::to_string(hovered_value).c_str(), { mouse_pos.x + 12, mouse_pos.y - 5 }, 185, 185, 185, 255);
  }

  groupbox_offset.y += 30;
}


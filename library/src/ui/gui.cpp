#include <common.hpp>
#include <hooks.hpp>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void ui::gui::initialize_imgui() {
  ImGui::CreateContext();

  auto io = ImGui::GetIO();
  io.IniFilename = nullptr;
  io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
  io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\tahoma.ttf)", 14.f);

  ImGui_ImplWin32_Init(hooks::wndproc::window);
  ImGui_ImplDX11_Init(hooks::present::device, hooks::present::context);
}

long long ui::gui::handle_message(HWND window, unsigned int message, unsigned long long wparam, long long lparam) {
  return ImGui_ImplWin32_WndProcHandler(window, message, wparam, lparam);
}

void ui::gui::begin() {
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();
}

void ui::gui::end() {
  ImGui::EndFrame();
  ImGui::Render();

  hooks::present::context->OMSetRenderTargets(1, &hooks::present::render_target, nullptr);
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ui::gui::add_window(const char* name) {
  ui::gui::mouse_clicked = ImGui::IsMouseDown(ImGuiMouseButton_Left);
  auto mouse_position = ImGui::GetMousePos();

  if (ui::gui::mouse_dragging && !ui::gui::mouse_clicked)
    ui::gui::mouse_dragging = false;

  if (ui::gui::mouse_dragging && ui::gui::mouse_clicked) {
    ui::gui::position_x = mouse_position.x - ui::gui::mouse_drag_x;
    ui::gui::position_y = mouse_position.y - ui::gui::mouse_drag_y;
  }

  if (ImGui::IsMouseHoveringRect({ ui::gui::position_x, ui::gui::position_y }, { ui::gui::position_x + ui::gui::menu_width, ui::gui::position_y + 20 })) {
    ui::gui::mouse_dragging = true;

    ui::gui::mouse_drag_x = mouse_position.x - ui::gui::position_x;
    ui::gui::mouse_drag_y = mouse_position.y - ui::gui::position_y;
  }

  ui::gui::add_rectangle(ui::gui::position_x, ui::gui::position_y, ui::gui::menu_width, ui::gui::menu_height, 50, 50, 50, 255);
  ui::gui::add_rectangle(ui::gui::position_x, ui::gui::position_y + 20, ui::gui::menu_width, 1, 195, 141, 146, 255);
  ui::gui::add_outlined_rectangle(ui::gui::position_x, ui::gui::position_y, ui::gui::menu_width, ui::gui::menu_height, 0, 0, 0, 85);
  ui::gui::add_text(name, ui::gui::position_x + 5, ui::gui::position_y + 2, 180, 180, 180, 255);
}

void ui::gui::add_groupbox(const char* name, float x, float y, float width, float height) {
  auto text_size = ImGui::CalcTextSize(name);

  ui::gui::add_outlined_rectangle(ui::gui::position_x + x, ui::gui::position_y + y, width, height, 20, 20, 20, 255);
  ui::gui::add_outlined_rectangle(ui::gui::position_x + x + 1, ui::gui::position_y + y + 1, width - 2, height - 2, 70, 70, 70, 255);
  ui::gui::add_rectangle(ui::gui::position_x + x + width / 2 - text_size.x / 2 - 3, ui::gui::position_y + y, text_size.x + 6, 2, 44, 44, 44, 255);
  ui::gui::add_text(name, ui::gui::position_x + x + width / 2 - text_size.x / 2, ui::gui::position_y + y - 6, 185, 185, 185, 255);

  ui::gui::groupbox_offset_y = ui::gui::position_y + y + 12;
  ui::gui::groupbox_offset_x = ui::gui::position_x + x;

  ui::gui::groupbox_bottom = ui::gui::position_y + y + height;
  ui::gui::groupbox_top = ui::gui::position_y + y;
  ui::gui::groupbox_width = ui::gui::position_x + x + width;
}

void ui::gui::add_checkbox(const char* name, bool* item) {
  if (ui::gui::groupbox_bottom <= ui::gui::groupbox_offset_y + 18)
    return;

  if (ui::gui::groupbox_top >= ui::gui::groupbox_offset_y + 18)
    return;

  auto text_size = ImGui::CalcTextSize(name);

  if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect({ ui::gui::groupbox_offset_x + 7, ui::gui::groupbox_offset_y }, { ui::gui::groupbox_offset_x + 7 + 12 + text_size.x, ui::gui::groupbox_offset_y + 12 }))
    *item = !*item;

  if (*item)
    ui::gui::add_rectangle(ui::gui::groupbox_offset_x + 8, ui::gui::groupbox_offset_y, 12, 12, 195, 141, 145, 255);
  else
    ui::gui::add_rectangle(ui::gui::groupbox_offset_x + 8, ui::gui::groupbox_offset_y, 12, 12, 50, 50, 50, 255);

  ui::gui::add_outlined_rectangle(ui::gui::groupbox_offset_x + 8, ui::gui::groupbox_offset_y, 12, 12, 101, 101, 101, 255);
  ui::gui::add_text(name, ui::gui::groupbox_offset_x + 29, ui::gui::groupbox_offset_y - 2, 185, 185, 185, 255);

  if (ImGui::IsMouseHoveringRect({ ui::gui::groupbox_offset_x + 7, ui::gui::groupbox_offset_y }, { ui::gui::groupbox_offset_x + 7 + 12 + text_size.x, ui::gui::groupbox_offset_y + 12 }))
    ui::gui::add_outlined_rectangle(ui::gui::groupbox_offset_x + 9, ui::gui::groupbox_offset_y + 1, 10, 10, 101, 101, 101, 255);

  ui::gui::groupbox_offset_y += 18;
}

void ui::gui::add_slider(const char* name, int min, int max, int* item, int step) {
  if (ui::gui::groupbox_bottom <= ui::gui::groupbox_offset_y + 30)
    return;

  auto pixel_size = (max - min) / 200.f;
  auto mouse_position = ImGui::GetMousePos();
  auto hovered_value = (int)(std::abs((int)(mouse_position.x - (ui::gui::groupbox_offset_x + 29))) * pixel_size);

  if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect({ ui::gui::groupbox_offset_x + 29, ui::gui::groupbox_offset_y + 14 }, { ui::gui::groupbox_offset_x + 29 + 201, ui::gui::groupbox_offset_y + 14 + 10 }))
    *item = hovered_value;

  if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect({ ui::gui::groupbox_offset_x + 21, ui::gui::groupbox_offset_y + 12 }, { ui::gui::groupbox_offset_x + 21 + 8, ui::gui::groupbox_offset_y + 12 + 10 }))
    *item -= step;

  if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect({ ui::gui::groupbox_offset_x + 22 + 200, ui::gui::groupbox_offset_y + 12 }, { ui::gui::groupbox_offset_x + 22 + 200 + 15, ui::gui::groupbox_offset_y + 12 + 10 }))
    *item += step;

  if (*item > max)
    *item = max;

  if (*item < min)
    *item = min;

  auto text = std::string(name) + " [" + std::to_string(*item) + "]";
  ui::gui::add_text(text.c_str(), ui::gui::groupbox_offset_x + 29, ui::gui::groupbox_offset_y - 3, 185, 185, 185, 255);
  ui::gui::add_rectangle(ui::gui::groupbox_offset_x + 29, ui::gui::groupbox_offset_y + 14, 200, 10, 50, 50, 50, 255);

  if (*item) {
    if (*item == max)
      ui::gui::add_rectangle(ui::gui::groupbox_offset_x + 29, ui::gui::groupbox_offset_y + 14, 200, 10, 195, 141, 145, 255);
    else
      ui::gui::add_rectangle(ui::gui::groupbox_offset_x + 29, ui::gui::groupbox_offset_y + 14, *item / pixel_size, 10, 195, 141, 145, 255);
  }

  ui::gui::add_outlined_rectangle(ui::gui::groupbox_offset_x + 29, ui::gui::groupbox_offset_y + 14, 200, 10, 101, 101, 101, 255);
  ui::gui::add_text("-", ui::gui::groupbox_offset_x + 22, ui::gui::groupbox_offset_y + 10, 185, 185, 185, 255);
  ui::gui::add_text("+", ui::gui::groupbox_offset_x + 22 + 200 + 12, ui::gui::groupbox_offset_y + 11, 185, 185, 185, 255);

  if (ImGui::IsMouseHoveringRect({ ui::gui::groupbox_offset_x + 29, ui::gui::groupbox_offset_y + 14 }, { ui::gui::groupbox_offset_x + 29 + 200 + 1, ui::gui::groupbox_offset_y + 14 + 10 })) {
    ui::gui::add_rectangle(ui::gui::groupbox_offset_x + 29, ui::gui::groupbox_offset_y + 14, hovered_value / pixel_size, 10, 101, 101, 101, 255);
    ui::gui::add_text(std::to_string(hovered_value).c_str(), mouse_position.x + 12, mouse_position.y - 5, 185, 185, 185, 255);
  }

  ui::gui::groupbox_offset_y += 30;
}

void ui::gui::add_text(const char* name, float x, float y, int r, int g, int b, int a) {
  auto draw_list = ImGui::GetBackgroundDrawList();

  draw_list->AddText({ x + 1, y + 1 }, ImColor{ 0, 0, 0, 255 }, name);
  draw_list->AddText({ x - 1, y - 1 }, ImColor{ 0, 0, 0, 255 }, name);
  draw_list->AddText({ x + 1, y - 1 }, ImColor{ 0, 0, 0, 255 }, name);
  draw_list->AddText({ x - 1, y + 1 }, ImColor{ 0, 0, 0, 255 }, name);
  
  draw_list->AddText({ x + 1, y }, ImColor{ 0, 0, 0, 255 }, name);
  draw_list->AddText({ x - 1, y }, ImColor{ 0, 0, 0, 255 }, name);
  draw_list->AddText({ x, y - 1 }, ImColor{ 0, 0, 0, 255 }, name);
  draw_list->AddText({ x, y + 1 }, ImColor{ 0, 0, 0, 255 }, name);
  
  draw_list->AddText({ x, y }, ImColor{ r, g, b, a }, name);
}

void ui::gui::add_rectangle(float x, float y, float width, float height, int r, int g, int b, int a) {
  ImGui::GetBackgroundDrawList()->AddRectFilled({ x, y }, { x + width, y + height }, ImColor{ r, g, b, a });
}

void ui::gui::add_outlined_rectangle(float x, float y, float width, float height, int r, int g, int b, int a) {
  ImGui::GetBackgroundDrawList()->AddRect({ x, y }, { x + width, y + height }, ImColor{ r, g, b, a });
}
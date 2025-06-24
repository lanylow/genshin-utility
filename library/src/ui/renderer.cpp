#include <ui/renderer.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

#include <hooks/hooks.hpp>

Renderer::Renderer() {
  inst_ = this;
}

void Renderer::Render(IDXGISwapChain* swap_chain, const std::function<void()>& callback) {
  Init(swap_chain);

  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();

  callback();

  ImGui::EndFrame();
  ImGui::Render();

  render_data_.context->OMSetRenderTargets(1, &render_data_.render_target, nullptr);
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::Resize() {
  render_data_.render_target->Release();
  render_data_.render_target = nullptr;
}

void Renderer::AddText(const std::string& text, ImVec2 pos, ImColor color) {
  const auto draw_list = ImGui::GetBackgroundDrawList();

  draw_list->AddText(ImVec2(pos.x + 1, pos.y + 1), IM_COL32_BLACK, text.c_str());
  draw_list->AddText(ImVec2(pos.x - 1, pos.y - 1), IM_COL32_BLACK, text.c_str());
  draw_list->AddText(ImVec2(pos.x + 1, pos.y - 1), IM_COL32_BLACK, text.c_str());
  draw_list->AddText(ImVec2(pos.x - 1, pos.y + 1), IM_COL32_BLACK, text.c_str());

  draw_list->AddText(ImVec2(pos.x + 1, pos.y), IM_COL32_BLACK, text.c_str());
  draw_list->AddText(ImVec2(pos.x - 1, pos.y), IM_COL32_BLACK, text.c_str());
  draw_list->AddText(ImVec2(pos.x, pos.y - 1), IM_COL32_BLACK, text.c_str());
  draw_list->AddText(ImVec2(pos.x, pos.y + 1), IM_COL32_BLACK, text.c_str());

  draw_list->AddText(ImVec2(pos.x, pos.y), color, text.c_str());
}

void Renderer::AddRectangle(ImVec2 pos, ImVec2 size, ImColor color) {
  ImGui::GetBackgroundDrawList()->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), color);
}

void Renderer::AddOutlinedRectangle(ImVec2 pos, ImVec2 size, ImColor color) {
  ImGui::GetBackgroundDrawList()->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), color);
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Renderer::WndProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam) {
  ImGui_ImplWin32_WndProcHandler(window, msg, wparam, lparam);
  
  if (inst_->render_data_.capture_input)
    return true;

  return CallWindowProcA(inst_->wnd_proc_, window, msg, wparam, lparam);
}

void Renderer::Init(IDXGISwapChain* swap_chain) {
  std::call_once(init_flag_, [&] {
    swap_chain->GetDevice(__uuidof(ID3D11Device), (void**)&render_data_.device); // NOLINT(clang-diagnostic-language-extension-token)
    render_data_.device->GetImmediateContext(&render_data_.context);

    auto swap_chain_desc = DXGI_SWAP_CHAIN_DESC();
    swap_chain->GetDesc(&swap_chain_desc);

    render_data_.window = swap_chain_desc.OutputWindow;
    inst_->wnd_proc_ = (WNDPROC)SetWindowLongPtrA(render_data_.window, GWLP_WNDPROC, (LONG_PTR)&WndProc);

    ImGui::CreateContext();

    auto io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
    io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\tahoma.ttf)", 14.f);

    ImGui_ImplWin32_Init(render_data_.window);
    ImGui_ImplDX11_Init(render_data_.device, render_data_.context);
  });

  if (!render_data_.render_target) {
    ID3D11Texture2D* back_buffer = nullptr;
    swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer); // NOLINT(clang-diagnostic-language-extension-token)
    render_data_.device->CreateRenderTargetView(back_buffer, nullptr, &render_data_.render_target);
    back_buffer->Release();
  }
}

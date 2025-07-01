#pragma once

#include <d3d11.h>

#include <mutex>
#include <functional>

#include <imgui/imgui.h>

struct RenderData {
  HWND window = nullptr;
  ID3D11Device* device = nullptr;
  ID3D11DeviceContext* context = nullptr;
  ID3D11RenderTargetView* render_target = nullptr;
  bool capture_input = false;
};

class Renderer {
public:
  Renderer();

  void Render(IDXGISwapChain* swap_chain, const std::function<void()>& callback);
  void Resize();

  void AddText(const std::string& text, ImVec2 pos, ImColor color) const;
  void AddRectangle(ImVec2 pos, ImVec2 size, ImColor color) const;
  void AddOutlinedRectangle(ImVec2 pos, ImVec2 size, ImColor color) const;

  RenderData& GetRenderData() { return render_data_; }

private:
  static LRESULT WndProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam);

  void Init(IDXGISwapChain* swap_chain);

  static inline Renderer* inst_ = nullptr;

  RenderData render_data_;
  std::once_flag init_flag_;
  WNDPROC wnd_proc_ = nullptr;
};

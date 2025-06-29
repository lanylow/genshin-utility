#pragma once

#include <d3d11.h>

#include <mutex>

#include <hooks/hook.hpp>

class GenshinUtility;

class Hooks {
public:
  Hooks(GenshinUtility* gu);

private:
  static HRESULT Present(IDXGISwapChain* _this, UINT sync_interval, UINT flags);
  static HRESULT ResizeBuffers(IDXGISwapChain* _this, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT format, UINT flags);
  static void SetFieldOfView(void* _this, float value);
  static void SetFieldOfViewGi(void* _this, float value);
  static void Quit();
  static void Enter(void* _this);
  static void Leave(void* _this, void* a1);

  static inline Hooks* inst_ = nullptr;

  GenshinUtility* gu_;
  std::once_flag present_flag_;

  Hook<decltype(&Present)> present_;
  Hook<decltype(&ResizeBuffers)> resize_buffers_;
  Hook<decltype(&SetFieldOfView)> set_field_of_view_;
  Hook<decltype(&Quit)> quit_;
  Hook<decltype(&Enter)> enter_;
  Hook<decltype(&Leave)> leave_;
};

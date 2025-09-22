#pragma once

#include <d3d11.h>

#include <mutex>

#include <hooks/hook.hpp>

class GenshinUtility;

class Hooks {
public:
  Hooks(GenshinUtility* gu);
  
private:
  HRESULT Present(IDXGISwapChain* _this, UINT sync_interval, UINT flags);
  HRESULT ResizeBuffers(IDXGISwapChain* _this, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT format, UINT flags);
  void SetFieldOfView(void* _this, float value);
  void SetFieldOfViewGi(void* _this, float value);
  void Quit();
  void Enter(void* _this);
  void Leave(void* _this, void* a1);
  
  InlineHook present_;
  InlineHook resize_buffers_;
  InlineHook set_field_of_view_;
  VehHook set_field_of_view_gi_;
  InlineHook quit_;
  InlineHook enter_;
  InlineHook leave_;
  
  GenshinUtility* gu_;
  std::once_flag present_flag_;
};

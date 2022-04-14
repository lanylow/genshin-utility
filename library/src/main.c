#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "../lib/minhook/MinHook.h"

#define HANDLE_KEY(key_code, var, step, min, max) \
  if ((GetAsyncKeyState(key_code) & 1) && (var + step >= min) && (var + step <= max)) { \
    var += step;                                                                        \
    redraw_console();                                                                   \
  }

typedef void*(__cdecl *il2cpp_resolve_icall_t)(const char* name);
il2cpp_resolve_icall_t il2cpp_resolve_icall = NULL;

typedef void(__cdecl* camera_set_field_of_view_t)(void* camera, float value, void* info);
camera_set_field_of_view_t camera_set_field_of_view = NULL;

typedef void(__cdecl* application_set_target_frame_rate_t)(int value, void* info);
application_set_target_frame_rate_t application_set_target_frame_rate = NULL;

typedef void(__cdecl* quality_settings_set_vsync_count_t)(int value, void* info);
quality_settings_set_vsync_count_t quality_settings_set_vsync_count = NULL;

int target_fov = 45;
int target_fps = 120;

void camera_set_field_of_view_h(void* camera, float value, void* info) {
  if (floor((double)value) == 45.0)
    value = (float)target_fov;

  application_set_target_frame_rate(target_fps, NULL);
  quality_settings_set_vsync_count(0, NULL);

  camera_set_field_of_view(camera, value, info);
}

void redraw_console() {
  COORD coord = { 0, 0 };
  HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleCursorPosition(console, coord);
  char buff[2048];
  sprintf(buff, "Genshin Utility created by lanylow\r\n"
                "Press insert/delete to increase/decrease field of view\r\n"
                "Press home/end to increase/decrease FPS cap\r\n"
                "Field of view: %d   \r\n"
                "FPS cap: %d   \r\n", target_fov, target_fps);
  WriteConsoleA(console, buff, strlen(buff), NULL, NULL);
}

unsigned long __stdcall main_thread(void* arg) {
  AllocConsole();
  freopen_s((FILE**)stdout, "conout$", "w", stdout);

  HANDLE unity_player = NULL;
  while (!(unity_player = GetModuleHandleA("UnityPlayer.dll"))) Sleep(100);

  HANDLE user_assembly = NULL;
  while (!(user_assembly = GetModuleHandleA("UserAssembly.dll"))) Sleep(100);

  il2cpp_resolve_icall = (il2cpp_resolve_icall_t)GetProcAddress(user_assembly, "il2cpp_resolve_icall");
  camera_set_field_of_view = (camera_set_field_of_view_t)il2cpp_resolve_icall("UnityEngine.Camera::set_fieldOfView()");
  application_set_target_frame_rate = (application_set_target_frame_rate_t)il2cpp_resolve_icall("UnityEngine.Application::set_targetFrameRate(int)");
  quality_settings_set_vsync_count = (quality_settings_set_vsync_count_t)il2cpp_resolve_icall("UnityEngine.QualitySettings::set_vSyncCount(int)");

  MH_Initialize();
  MH_CreateHook(camera_set_field_of_view, (void*)(&camera_set_field_of_view_h), (void**)(&camera_set_field_of_view));
  MH_EnableHook(MH_ALL_HOOKS);

  redraw_console();

  while (1) {
    HANDLE_KEY(VK_INSERT, target_fov, 5, 5, 175);
    HANDLE_KEY(VK_DELETE, target_fov, -5, 5, 175);

    HANDLE_KEY(VK_HOME, target_fps, 10, 10, 360);
    HANDLE_KEY(VK_END, target_fps, -10, 10, 360);

    Sleep(10);
  }

  return 0;
}

int __stdcall DllMain(void* instance, unsigned long reason, void* reserved) {
  if (reason != DLL_PROCESS_ATTACH)
    return 0;

  HANDLE thread = INVALID_HANDLE_VALUE;
  if ((thread = CreateThread(NULL, 0, &main_thread, NULL, 0, NULL)))
    CloseHandle(thread);

  return 1;
}
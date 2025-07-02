#include <hooks/hook.hpp>

#include <map>
#include <ranges>
#include <mutex>

#include <minhook/MinHook.h>

namespace {
  auto veh_hooks = std::map<void*, void*>();

  LONG WINAPI ExceptionHandler(EXCEPTION_POINTERS* info) {
    if (info->ExceptionRecord->ExceptionCode == EXCEPTION_GUARD_PAGE) {
      for (const auto& [target, detour] : veh_hooks)
        if (info->ExceptionRecord->ExceptionAddress == target)
          info->ContextRecord->Rip = (uintptr_t)detour;

      info->ContextRecord->EFlags |= PAGE_GUARD;

      return EXCEPTION_CONTINUE_EXECUTION;
    }

    if (info->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP) {
      auto old = 0ul;

      for (const auto target : std::views::keys(veh_hooks))
        VirtualProtect(target, utils::GetPageSize(), PAGE_EXECUTE_READ | PAGE_GUARD, &old);

      return EXCEPTION_CONTINUE_EXECUTION;
    }

    return EXCEPTION_CONTINUE_SEARCH;
  }
}

void InlineHookProvider::Create(void* target, void* detour) {
  static auto init = MH_Initialize();

  target_ = target;
  MH_CreateHook(target, detour, &trampoline_);
  MH_EnableHook(target);
}

void InlineHookProvider::Remove() {
  MH_DisableHook(target_);
  MH_RemoveHook(target_);
}

void VehHookProvider::Create(void* target, void* detour) {
  static auto init = AddVectoredExceptionHandler(1, ExceptionHandler);

  target_ = target;
  veh_hooks.emplace(target, detour);

  auto old = 0ul;
  VirtualProtect(target, utils::GetPageSize(), PAGE_EXECUTE_READ | PAGE_GUARD, &old);
}

void VehHookProvider::Remove() {
  auto old = 0ul;
  VirtualProtect(target_, utils::GetPageSize(), PAGE_EXECUTE_READ, &old);

  veh_hooks.erase(target_);
}

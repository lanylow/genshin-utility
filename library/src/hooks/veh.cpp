#include <hooks/veh.hpp>

namespace veh {
  long exception_handler(EXCEPTION_POINTERS* info);
}

long veh::exception_handler(EXCEPTION_POINTERS* info) {
  switch (info->ExceptionRecord->ExceptionCode) {
  case EXCEPTION_GUARD_PAGE:
    if (info->ExceptionRecord->ExceptionAddress == data.target)
      info->ContextRecord->Rip = (uintptr_t)data.detour;

    info->ContextRecord->EFlags |= PAGE_GUARD;

    return EXCEPTION_CONTINUE_EXECUTION;

  case EXCEPTION_SINGLE_STEP:
    unsigned long old;
    VirtualProtect(data.target, data.system_info.dwPageSize, PAGE_EXECUTE_READ | PAGE_GUARD, &old);

    return EXCEPTION_CONTINUE_EXECUTION;

  default:
    return EXCEPTION_CONTINUE_SEARCH;
  }
}

void veh::initialize(void* target, void* detour) {
  data.target = target;
  data.detour = detour;

  GetSystemInfo(&data.system_info);
  data.handle = AddVectoredExceptionHandler(1, exception_handler);

  unsigned long old;
  VirtualProtect(target, data.system_info.dwPageSize, PAGE_EXECUTE_READ | PAGE_GUARD, &old);
}

void veh::destroy() {
  unsigned long old;
  VirtualProtect(data.target, data.system_info.dwPageSize, PAGE_EXECUTE_READ, &old);

  RemoveVectoredExceptionHandler(data.handle);
  data.handle = nullptr;
}

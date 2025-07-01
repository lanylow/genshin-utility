#pragma once

#include <type_traits>

#include <utils/utils.hpp>

class InlineHookProvider {
public:
  void Create(void* target, void* detour);
  void Remove();

  template <typename ReturnType, typename... Args>
  ReturnType CallOriginal(Args... args) const {
    return reinterpret_cast<ReturnType(*)(Args...)>(trampoline_)(args...);
  }

private:
  void* target_ = nullptr;
  void* trampoline_ = nullptr;
};

class VehHookProvider {
public:
  void Create(void* target, void* detour);
  void Remove();

  template <typename ReturnType, typename... Args>
  ReturnType CallOriginal(Args... args) const {
    const auto page_size = utils::GetPageSize();
    auto old = 0ul;

    if constexpr (std::is_void_v<ReturnType>) {
      VirtualProtect(target_, page_size, PAGE_EXECUTE_READ, &old);
      reinterpret_cast<ReturnType(*)(Args...)>(target_)(args...);
      VirtualProtect(target_, page_size, old, &old);
      return;
    }
    else {
      VirtualProtect(target_, page_size, PAGE_EXECUTE_READ, &old);
      const auto res = reinterpret_cast<ReturnType(*)(Args...)>(target_)(args...);
      VirtualProtect(target_, page_size, old, &old);
      return res;
    }
  }

private:
  void* target_ = nullptr;
};

template <class HookProvider>
class Hook {
public:
  template <typename TargetType, typename DetourType>
  void Install(TargetType target, DetourType detour) {
    provider_.Create((void*)target, (void*)detour);
  }

  template <typename DetourType>
  void InstallSwapChain(int index, DetourType detour) {
    provider_.Create(utils::GetSwapChainMethod(index), (void*)detour);
  }

  void Remove() {
    provider_.Remove();
  }

  template <typename ReturnType, typename... Args>
  ReturnType CallOriginal(Args... args) const {
    return provider_.template CallOriginal<ReturnType, Args...>(args...);
  }

private:
  HookProvider provider_;
};

using InlineHook = Hook<InlineHookProvider>;
using VehHook = Hook<VehHookProvider>;

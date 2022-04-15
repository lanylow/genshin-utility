#pragma once

namespace GenshinUtility {
  class GIl2Cpp final : public GInstance<GIl2Cpp> {
  public:
    void Init() noexcept;

    template <typename T>
    const T GetMethod(std::string signature) noexcept {
      if (m_methodCache.find(signature) != m_methodCache.end())
        return reinterpret_cast<T>(m_methodCache[signature]);

      void* function = RunCdecl<void*>(m_resolveInternalCall, signature.c_str());
      m_methodCache[signature] = function;
      return reinterpret_cast<T>(function);
    }

    template <typename T, typename... TArgs>
    static T RunCdecl(const void* function, TArgs... args) noexcept {
      return reinterpret_cast<T(__cdecl*)(TArgs...)>(function)(std::forward<TArgs>(args)...);
    }

  private:
    void* m_resolveInternalCall;
    void* m_userAssembly;
    std::unordered_map<std::string, void*> m_methodCache;
  };
}
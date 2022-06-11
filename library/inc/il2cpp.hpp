#pragma once

namespace GenshinUtility {
  class GIl2Cpp final : public GInstance<GIl2Cpp> {
  public:
    void Init() noexcept;

    template <typename T, typename... TArgs>
    static T RunCdecl(const void* function, TArgs... args) noexcept {
      return reinterpret_cast<T(__cdecl*)(TArgs...)>(function)(std::forward<TArgs>(args)...);
    }

    void* m_setFieldOfView;
    void* m_targetFrameRate;
    void* m_setVsyncCount;
    void* m_setFog;

  private:
    void* m_unityPlayer;
  };
}
#pragma once

namespace GenshinUtility {
  template <typename T>
  class GInstance {
  protected:
    GInstance() noexcept { }
    ~GInstance() noexcept { }

    GInstance(const GInstance&) noexcept = delete;
    GInstance& operator=(const GInstance&) noexcept = delete;

    GInstance(GInstance&&) noexcept = delete;
    GInstance& operator=(GInstance&&) noexcept = delete;

  public:
    static T* Get() noexcept {
      static T instance{ };
      return &instance;
    }
  };
}
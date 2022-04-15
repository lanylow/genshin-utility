#pragma once

namespace GenshinUtility {
  class FWindows final {
  public:
    enum class EDllEvents : UInt32 {
      ProcessDetach,
      ProcessAttach,
      ThreadAttach,
      ThreadDetach
    };

    static void NewThread(const void* startRoutine) noexcept {
      CreateThread(nullptr, 0, reinterpret_cast<PTHREAD_START_ROUTINE>(startRoutine), nullptr, 0, nullptr);
    }
  };
}
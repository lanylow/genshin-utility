#pragma once

#include <functional>
#include <atomic>

namespace utils {
  struct once_flag {
    explicit once_flag(bool init = true)
      : value(init) {}

    void reset() {
      value = true;
    }

    utils::once_flag& operator=(bool val) {
      value = val;
      return *this;
    }

    explicit operator bool() const {
      return value.load();
    }

  private:
    mutable std::atomic_bool value = true;
  };

  template <typename fn_type, class... args_type>
    requires std::is_invocable_v<fn_type, args_type...>
  void call_once(utils::once_flag& flag, fn_type&& function, args_type&&... args) {
    if (!flag)
      return;

    std::invoke(std::forward<fn_type>(function), std::forward<args_type>(args)...);

    flag = false;
  }
}

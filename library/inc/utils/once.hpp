#pragma once

#include <functional>
#include <atomic>

namespace utils {
  template <typename function_type, typename... args_type>
  struct is_invocable_with :
    std::conjunction<
      std::is_invocable<function_type, args_type...>,
      std::conditional_t<
        std::is_class_v<std::remove_reference_t<function_type>>,
        std::is_convertible<std::remove_reference_t<function_type>, std::function<void(args_type...)>>,
        std::true_type
      >
    > { };

  struct once_flag {
    explicit constexpr once_flag(bool init = true) : value(init) { }

    void reset() {
      value = true;
    }

    utils::once_flag& operator=(const bool val) {
      value = val;
      return *this;
    }

    explicit operator bool() const {
      return value.load();
    }

  private:
    mutable std::atomic_bool value{ true };
  };

  template <typename function_type, class... args_type,
    std::enable_if_t<utils::is_invocable_with<function_type, args_type...>::value, int> = 0>
  constexpr void call_once(utils::once_flag& flag, function_type&& function, args_type&&... args) {
    if (!flag)
      return;

    std::invoke(std::forward<function_type>(function), std::forward<args_type>(args)...);

    flag = false;
  }
}
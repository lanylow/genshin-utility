#pragma once

#include <type_traits>

namespace utils {
  template <typename type>
  concept fn_convertible = std::is_integral_v<type> || std::is_pointer_v<type>;

  template <typename return_type>
    requires std::is_default_constructible_v<return_type> || std::is_void_v<return_type>
  class function {
  public:
    function() = default;

    template <utils::fn_convertible cast_type>
    explicit function(cast_type address)
      : address((uintptr_t)address) {}

    template <typename... arg_type>
    return_type invoke(arg_type&&... args) const {
      if (address)
        return ((return_type(*)(arg_type...))address)(std::forward<arg_type>(args)...);

      if constexpr (std::is_void_v<return_type>)
        return;
      else
        return return_type{};
    }

    template <typename... arg_type>
    return_type operator()(arg_type&&... args) const {
      return invoke(std::forward<arg_type>(args)...);
    }

    template <utils::fn_convertible cast_type>
    utils::function<return_type>& operator=(cast_type new_address) {
      address = (uintptr_t)new_address;
      return *this;
    }

    template <utils::fn_convertible cast_type>
    explicit operator cast_type() const {
      return (cast_type)address;
    }

  private:
    uintptr_t address = 0ll;
  };
}

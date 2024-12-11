#pragma once

#include <type_traits>

namespace utils {
  template <typename cast_type>
  struct is_fn_convertible :
    std::disjunction<
      std::is_integral<cast_type>,
      std::is_pointer<cast_type>
    > { };

  template <typename return_type, 
    std::enable_if_t<std::is_default_constructible_v<return_type> || std::is_void_v<return_type>, int> = 0>
  class function {
  public:
    function() = default;

    template <typename cast_type,
      std::enable_if_t<utils::is_fn_convertible<cast_type>::value, int> = 0>
    explicit function(cast_type address)
      : address((unsigned long long)(address)) { }

    template <typename... arg_type>
    return_type invoke(arg_type&&... args) const {
      if (address)
        return ((return_type(*)(arg_type...))(address))(std::forward<arg_type>(args)...);

      if constexpr (std::is_void_v<return_type>)
        return;
      else
        return return_type{ };
    }

    template <typename... arg_type>
    return_type operator()(arg_type&&... args) const {
      return invoke(std::forward<arg_type>(args)...);
    }

    template <typename cast_type,
      std::enable_if_t<utils::is_fn_convertible<cast_type>::value, int> = 0>
    utils::function<return_type>& operator=(cast_type new_address) {
      address = (unsigned long long)(new_address);
      return *this;
    }
    
    template <typename cast_type,
      std::enable_if_t<utils::is_fn_convertible<cast_type>::value, int> = 0>
    explicit operator cast_type() const {
      return (cast_type)(address);
    }

  private:
    unsigned long long address{ 0 };
  };
}
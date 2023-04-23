#pragma once

namespace utils {
  template <typename return_type, 
    std::enable_if_t<std::is_arithmetic_v<return_type> || std::is_void_v<return_type> || std::is_pointer_v<return_type>, int> = 0>
  class fn {
  public:
    constexpr fn() = default;
    explicit constexpr fn(unsigned long long address) : address(address) { }

    template <typename... arg_type>
    constexpr return_type invoke(arg_type&&... args) {
      if (!address)
        return return_type{ };

      return reinterpret_cast<return_type(*)(arg_type...)>(address)(std::forward<arg_type>(args)...);
    }

    template <typename... arg_type>
    constexpr return_type operator()(arg_type&&... args) {
      return invoke(std::forward<arg_type>(args)...);
    }

    constexpr utils::fn<return_type>& operator=(unsigned long long new_address) { 
      address = new_address;
      return *this;
    }
    
    template <typename cast_type,
      std::enable_if_t<std::is_integral_v<cast_type> || std::is_pointer_v<cast_type>, int> = 0>
    constexpr operator cast_type() const {
      return (cast_type)(address);
    }

  private:
    unsigned long long address{ };
  };
}
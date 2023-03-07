#pragma once

namespace utils {
  template <typename return_type>
  class fn {
  public:
    fn() = default;
    fn(unsigned long long address) : address(address) { }

    template <typename... arg_type>
    inline return_type invoke(arg_type... args) {
      return reinterpret_cast<return_type(*)(arg_type...)>(address)(std::forward<arg_type>(args)...);
    }

    template <typename... arg_type>
    inline return_type operator()(arg_type... args) {
      return invoke(std::forward<arg_type>(args)...);
    }

  private:
    unsigned long long address{ };
  };
}
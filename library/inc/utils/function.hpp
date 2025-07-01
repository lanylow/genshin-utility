#pragma once

#include <type_traits>

template <typename ReturnType>
class Function {
public:
  Function() = default;

  template <typename AddressType>
  explicit Function(AddressType address)
    : address_((uintptr_t)address) {}

  template <typename... Args>
  ReturnType Invoke(Args... args) const {
    if (address_)
      return reinterpret_cast<ReturnType(*)(Args...)>(address_)(args...);

    if constexpr (std::is_void_v<ReturnType>)
      return;
    else
      return ReturnType{};
  }

  template <typename... Args>
  ReturnType operator()(Args... args) const { return Invoke(args...); }

  template <typename AddressType>
  Function& operator=(AddressType value) {
    address_ = (uintptr_t)value;
    return *this;
  }

  template <typename AddressType>
  explicit operator AddressType() const { return (AddressType)address_; }

private:
  uintptr_t address_ = 0ll;
};

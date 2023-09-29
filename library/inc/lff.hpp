#pragma once

#include <unordered_map>
#include <cstdint>
#include <any>
#include <optional>
#include <filesystem>

/*
 * Lanylow's file format
 */

namespace lff {
  using hash_t = unsigned short;
  using type_t = unsigned char;
}

namespace lff::detail {
  struct value_type {
    enum {
      bool8,
      int32
    };

    template <typename type>
    constexpr static lff::type_t from_type() {
      if (typeid(type) == typeid(bool))
        return lff::detail::value_type::bool8;

      return lff::detail::value_type::int32;
    }
  };

  struct data_entry {
    lff::type_t type{ lff::detail::value_type::int32 };
    std::any value{ };
  };

  constexpr lff::hash_t get_hash(const char* data, lff::hash_t value = 0x811c) { // NOLINT(*-no-recursion)
    if (data[0] == '\0')
      return value;

    return lff::detail::get_hash(&data[1], (value ^ lff::hash_t(data[0])) * 0x111);
  }

  const unsigned short signature = 0x775d;
}

namespace lff {
  class data_container {
  public:
    data_container() = default;

    void set_value(lff::hash_t hash, lff::type_t type, std::any value) {
      values[hash] = { type, std::move(value) };
    }

    template <typename cast_type,
      std::enable_if_t<std::is_default_constructible_v<cast_type>, int> = 0>
    void set_value(const char* name, cast_type value) {
      set_value(lff::detail::get_hash(name), lff::detail::value_type::from_type<cast_type>(), std::make_any<cast_type>(value));
    }

    template <typename cast_type>
    std::optional<cast_type> get_value(lff::hash_t hash) {
      try {
        return std::make_optional(std::any_cast<cast_type>(values[hash].value));
      }
      catch (std::exception&) {
        return std::nullopt;
      }
    }

    template <typename cast_type>
    std::optional<cast_type> get_value(const char* name) {
      return get_value<cast_type>(lff::detail::get_hash(name));
    }

    std::unordered_map<lff::hash_t, lff::detail::data_entry> values{ };
  };

  std::optional<lff::data_container> parse(const std::filesystem::path& path);

  void write(lff::data_container& container, const std::filesystem::path& path);
}

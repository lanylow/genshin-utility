#include "lff.hpp"

#include <fstream>

template <typename type>
inline type read_buffer(unsigned char** buffer) {
  auto value = *(type*)(*buffer);
  *buffer += sizeof(type);
  return value;
}

std::optional<lff::data_container> lff::parse(const std::filesystem::path& path) {
  std::ifstream input(path, std::ios::in | std::ios::binary);
  if (!input.is_open() || !input.good())
    return std::nullopt;

  input.seekg(0, std::ios::end);
  auto size = input.tellg();
  if (size == -1)
    return std::nullopt;

  auto data = new unsigned char[size];
  input.seekg(0, std::ios::beg);
  input.read((char*)(data), size);
  input.close();

  auto signature = read_buffer<unsigned short>(&data);
  if (signature != lff::detail::signature)
    return std::nullopt;

  lff::data_container container;
  for (auto i = data; i < data + size;) {
    auto hash = read_buffer<lff::hash_t>(&i);
    auto type = read_buffer<lff::type_t>(&i);

    std::any value;
    switch (type) {
      case lff::detail::value_type::bool8:
        value = std::make_any<bool>(read_buffer<bool>(&i));
        break;

      case lff::detail::value_type::int32:
        value = std::make_any<int>(read_buffer<int>(&i));
        break;

      default: break;
    }

    container.set_value(hash, type, value);
  }

  return std::make_optional<lff::data_container>(container);
}

template <typename type>
inline void write_buffer(std::ofstream& stream, type value) {
  stream.write((const char*)(&value), sizeof(type));
}

void lff::write(lff::data_container& container, const std::filesystem::path& path) {
  std::ofstream output(path, std::ios::out | std::ios::binary);
  if (!output.is_open() || !output.good())
    return;

  write_buffer(output, lff::detail::signature);

  for (auto& [hash, entry] : container.values) {
    write_buffer(output, hash);
    write_buffer(output, entry.type);

    switch (entry.type) {
      case lff::detail::value_type::bool8:
        write_buffer(output, container.get_value<bool>(hash).value());
        break;

      case lff::detail::value_type::int32:
        write_buffer(output, container.get_value<int>(hash).value());
        break;

      default: break;
    }
  }

  output.close();
}
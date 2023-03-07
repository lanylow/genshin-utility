#include <common.hpp>

void config::load() {
  if (!std::filesystem::is_directory(config::config_folder)) {
    std::filesystem::remove(config::config_folder);

    if (!std::filesystem::create_directories(config::config_folder))
      return;
  }

  std::ifstream input_file(config::config_path.string(), std::ios::in);

  if (!input_file.good())
    return;

  nlohmann::json config = { };

  try {
    config = nlohmann::json::parse(input_file, nullptr, false);

    if (config.is_discarded())
      return;

    input_file.close();
  }
  catch (std::ifstream::failure&) {
    return;
  }

  try {
    variables::menu::open_on_start = config["openMenuOnStart"];
    variables::tools::fps_counter = config["fpsCounter"];
    variables::tools::enable_vsync = config["enableVSync"];
    variables::tools::disable_fog = config["disableFog"];
    variables::tools::fps_limit = config["fpsLimit"];
    variables::tools::camera_fov = config["cameraFov"];
  }
  catch (nlohmann::detail::exception&) {
    // ignored
  }
}

void config::save() {
  nlohmann::json config = { };

  try {
    config["openMenuOnStart"] = variables::menu::open_on_start;
    config["fpsCounter"] = variables::tools::fps_counter;
    config["enableVSync"] = variables::tools::enable_vsync;
    config["disableFog"] = variables::tools::disable_fog;
    config["fpsLimit"] = variables::tools::fps_limit;
    config["cameraFov"] = variables::tools::camera_fov;
  }
  catch (nlohmann::detail::exception&) {
    return;
  }

  std::ofstream output_file(config::config_path.string(), std::ios::out | std::ios::trunc);

  if (!output_file.good())
    return;

  try {
    output_file << config.dump();
    output_file.close();
  }
  catch (std::ofstream::failure&) {
    // ignored
  }
}

std::filesystem::path config::get_working_path() {
  std::filesystem::path path;

  if (wchar_t* path_to_documents; SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &path_to_documents))) {
    path.assign(path_to_documents);
    path.append("genshin-utility");
    CoTaskMemFree(path_to_documents);
  }

  return path;
}
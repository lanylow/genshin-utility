#include <shlobj.h>

#include <lff/lff.hpp>

#include <options.hpp>

void options::load() {
  if (!std::filesystem::is_directory(options::folder)) {
    std::filesystem::remove(options::folder);

    if (!std::filesystem::create_directories(options::folder))
      return;
  }

  auto data = lff::parse(options::path);

  if (!data.has_value())
    return;

  try {
    options::menu.open_on_start = data->get_value<bool>("openMenuOnStart").value();
    options::tools.fps_counter = data->get_value<bool>("fpsCounter").value();
    options::tools.enable_vsync = data->get_value<bool>("enableVSync").value();
    options::tools.disable_fog = data->get_value<bool>("disableFog").value();
    options::tools.fps_limit = data->get_value<int>("fpsLimit").value();
    options::tools.camera_fov = data->get_value<int>("cameraFov").value();
  }
  catch (std::exception&) {
    // ignored
  }
}

void options::save() {
  lff::data_container container;
  container.set_value("openMenuOnStart", options::menu.open_on_start);
  container.set_value("fpsCounter", options::tools.fps_counter);
  container.set_value("enableVSync", options::tools.enable_vsync);
  container.set_value("disableFog", options::tools.disable_fog);
  container.set_value("fpsLimit", options::tools.fps_limit);
  container.set_value("cameraFov", options::tools.camera_fov);

  try {
    lff::write(container, options::path);
  }
  catch (std::exception&) {
    // ignored
  }
}

std::filesystem::path options::get_working_path() {
  std::filesystem::path working_path;

  if (wchar_t* path_to_documents; SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &path_to_documents))) {
    working_path.assign(path_to_documents);
    working_path.append("genshin-utility");
    CoTaskMemFree(path_to_documents);
  }

  return working_path;
}

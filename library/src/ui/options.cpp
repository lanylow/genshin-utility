#include <ui/options.hpp>
#include <lff/lff.hpp>

#include <shlobj.h>

void ui::options::load() {
  if (!std::filesystem::is_directory(ui::options::folder)) {
    std::filesystem::remove(ui::options::folder);

    if (!std::filesystem::create_directories(ui::options::folder))
      return;
  }

  auto data = lff::parse(ui::options::path);

  if (!data.has_value())
    return;

  try {
    ui::options::menu::open_on_start = data->get_value<bool>("openMenuOnStart").value();
    ui::options::tools::fps_counter = data->get_value<bool>("fpsCounter").value();
    ui::options::tools::enable_vsync = data->get_value<bool>("enableVSync").value();
    ui::options::tools::disable_fog = data->get_value<bool>("disableFog").value();
    ui::options::tools::fps_limit = data->get_value<int>("fpsLimit").value();
    ui::options::tools::camera_fov = data->get_value<int>("cameraFov").value();
  }
  catch (std::exception&) {
    // ignored
  }
}

void ui::options::save() {
  lff::data_container container;
  container.set_value("openMenuOnStart", ui::options::menu::open_on_start);
  container.set_value("fpsCounter", ui::options::tools::fps_counter);
  container.set_value("enableVSync", ui::options::tools::enable_vsync);
  container.set_value("disableFog", ui::options::tools::disable_fog);
  container.set_value("fpsLimit", ui::options::tools::fps_limit);
  container.set_value("cameraFov", ui::options::tools::camera_fov);

  try {
    lff::write(container, ui::options::path);
  }
  catch (std::exception&) {
    // ignored
  }
}

std::filesystem::path ui::options::get_working_path() {
  std::filesystem::path working_path;

  if (wchar_t* path_to_documents; SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &path_to_documents))) {
    working_path.assign(path_to_documents);
    working_path.append("genshin-utility");
    CoTaskMemFree(path_to_documents);
  }

  return working_path;
}
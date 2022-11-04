#pragma once

#include <windows.h>
#include <d3d11.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <filesystem>
#include <shlobj.h>
#include <fstream>

namespace fs = std::filesystem;

#include <utilities/types.hpp>
#include <utilities/windows.hpp>
#include <utilities/instance.hpp>
#include <utilities/hook.hpp>
#include <gui/options.hpp>
#include <gui/gui.hpp>
#include <gui/menu.hpp>
#include <gui/fpscounter.hpp>
#include <il2cpp.hpp>
#include <hooks.hpp>
#include <config.hpp>

using namespace GenshinUtility;
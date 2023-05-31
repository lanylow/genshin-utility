#pragma once

#include <windows.h>
#include <cstdint>
#include <thread>
#include <d3d11.h>
#include <mutex>
#include <filesystem>
#include <fstream>
#include <shlobj.h>
#include <intrin.h>

#include <minhook/MinHook.h>
#include <kiero/kiero.h>
#include <json/json.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_internal.h>

#include "utils/fn.hpp"
#include "utils/mh.hpp"
#include "utils/once.hpp"

#include "unity/sdk.hpp"

#include "ui/variables.hpp"
#include "ui/gui.hpp"
#include "ui/menu.hpp"

#include "config.hpp"

using namespace std::chrono_literals;
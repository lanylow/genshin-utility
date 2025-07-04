project "Library"
  kind "SharedLib"
  language "C++"
  cppdialect "C++23"

  uses { "ImGui", "MinHook", "mINI" }
  links { "d3d11" }
  files { "inc/**.*", "src/**.*" }
  includedirs { "inc" }

  targetname "library"

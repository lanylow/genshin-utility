project "Library"
  kind "SharedLib"
  language "C++"
  cppdialect "C++23"

  uses { "ImGui", "MinHook", "mINI" }
  links { "d3d11", "shell32", "ole32" }
  files { "inc/**.*", "src/**.*" }
  includedirs { "inc" }

  targetname "library"

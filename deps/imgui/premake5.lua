project "ImGui"
  kind "StaticLib"
  language "C++"

  files { "inc/**.*", "src/**.*" }

  usage "PUBLIC"
    includedirs { "inc" }

  usage "INTERFACE"
    links { "ImGui" }

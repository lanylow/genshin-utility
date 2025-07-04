project "MinHook"
  kind "StaticLib"
  language "C"

  files { "inc/**.*", "src/**.*" }
  removefiles { "src/**32.*" }

  usage "PUBLIC"
    includedirs { "inc" }

  usage "INTERFACE"
    links { "MinHook" }

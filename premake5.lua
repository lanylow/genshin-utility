workspace "GenshinUtility"
  configurations { "Release" }
  architecture "x86_64"
  
  buildoptions { "/MP" }
  characterset "ASCII"
  staticruntime "On"
  runtime "Release"
  optimize "Speed"
  symbols "Off"
  
  location "_project"
  objdir "_bin/%{prj.name}/int"
  targetdir "_bin/%{prj.name}"

  startproject "Library"

include "deps"
include "src"

project "Loader"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++23"

  files { "src/**.*" }

  targetname "loader"

  filter { "system:windows", "configurations:Release" }
    postbuildcommands {
      [[echo ^<?xml version="1.0" encoding="UTF-8" standalone="yes"?^> > "$(TargetDir)app.manifest"]],
      [[echo ^<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0"^> >> "$(TargetDir)app.manifest"]],
      [[echo   ^<trustInfo xmlns="urn:schemas-microsoft-com:asm.v3"^> >> "$(TargetDir)app.manifest"]],
      [[echo     ^<security^> >> "$(TargetDir)app.manifest"]],
      [[echo       ^<requestedPrivileges^> >> "$(TargetDir)app.manifest"]],
      [[echo         ^<requestedExecutionLevel level="requireAdministrator" uiAccess="false"/^> >> "$(TargetDir)app.manifest"]],
      [[echo       ^</requestedPrivileges^> >> "$(TargetDir)app.manifest"]],
      [[echo     ^</security^> >> "$(TargetDir)app.manifest"]],
      [[echo   ^</trustInfo^> >> "$(TargetDir)app.manifest"]],
      [[echo ^</assembly^> >> "$(TargetDir)app.manifest"]],

      -- Embed manifest into exe (mt.exe is from Visual Studio tools)
      [[mt -manifest "$(TargetDir)app.manifest" -outputresource:"$(TargetPath)";#1]]
    }
  filter {}
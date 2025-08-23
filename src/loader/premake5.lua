project "Loader"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++23"

  files { "src/**.*" }

  targetname "loader"

  local function set_uac()
    require("vstudio")

    premake.override(premake.vstudio.vc2010.elements, "link", function(base, prj)
      local calls = base(prj)

      table.insert(calls, function()
        premake.vstudio.vc2010.element("UACExecutionLevel", nil, "requireAdministrator")
      end)

      return calls
    end)
  end

  set_uac()

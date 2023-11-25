-- Workspace Configuration
workspace "aiva"
    configurations { "debug" }

-- Project Configuration
project "aiva"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin"
    includedirs { "src" }
    files { "src/**.hpp", "src/**.cpp" }

-- Platform-Specific Settings
    -- Windows Configuration
    filter "system:windows"
        links { "SDL3", "SDL3main.dll" }

-- Build Configurations
    -- Debug Configuration
    filter "configurations:debug"
        defines { "DEBUG" }
        symbols "On"
        optimize "Off"

-- Compiler-Specific Settings
    -- Clang, GCC, and MinGW Options
    filter { "toolset:clang", "toolset:gcc", "toolset:mingw" }
        buildoptions { "-Weverything", "-Werror", "-pedantic" }

    -- MSC Options
    filter { "toolset:msc" }
        buildoptions { "/W4", "/WX" }

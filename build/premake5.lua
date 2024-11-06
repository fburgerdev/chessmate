-- premake5.lua
ROOT = ".."
WORKSPACE = "engine"
STARTUP_PROJECT = "server"

-- workspace
workspace(WORKSPACE)
    -- startproject
    startproject(STARTUP_PROJECT)
    -- cpp
    language "C++"
    cppdialect "C++20"

    -- debug
    debugger "GDB"

    -- defines
    defines { }

    -- dependancies
    -- :: directories
    libdirs {
        --[[ INSERT ADDITIONAL LINKS HERE ]]
    }
    -- :: libraries
    links { --[[ INSERT ADDITIONAL LINKS HERE ]] }

    -- config
    configurations { "debug", "release", "dist" }
    -- :: debug
    filter "configurations:debug"
        -- symbols / defines
        symbols "On"
        defines { "CONFIG_DEBUG" }
        -- options
        linkoptions{ }
    -- :: release
    filter "configurations:release"
        -- optimize / defines
        optimize "On"
        defines { "CONFIG_RELEASE" }
        -- options
        linkoptions{ "-Ofast" }
    -- :: dist
    filter "configurations:dist"
        -- optimize / defines
        optimize "On"
        defines { "CONFIG_DIST" }
        -- options
        linkoptions { "-Ofast" }
    
    -- system
    -- :: windows
    filter "system:windows"
        defines { "SYSTEM_WINDOWS" }
    -- :: linux
    filter "system:linux"
        defines { "SYSTEM_LINUX" }

    -- binaries
    targetdir(ROOT .. "/bin/%{cfg.buildcfg}")
    objdir(ROOT .. "/bin/obj")

    -- toolset
    -- :: gcc
    filter "toolset:gcc"
        buildoptions { "-Wall", "-Wextra", "-Wpedantic" }

-- project lib
project(WORKSPACE)
    -- static lib
    kind "StaticLib"

    -- precompiled headers
    pchheader "common.hpp"
    pchsource "common.cpp"
    
    -- include
    includedirs {
        ROOT .. "/",
        ROOT .. "/src",
        ROOT .. "/src/engine",
    }
    -- files
    files {
        ROOT .. "/src/engine/**",
    }

-- project server
project "server"
    -- console
    kind "ConsoleApp"

    -- include
    includedirs {
        ROOT .. "/",
        ROOT .. "/src",
        ROOT .. "/src/engine",
        ROOT .. "/src/server",
        ROOT .. "/vendor",
        --[[ INSERT ADDITIONAL DIRECTORIES HERE ]]
    }
    -- files
    files {
        ROOT .. "/src/server/**",
        --[[ INSERT ADDITIONAL FILES HERE ]]
    }

    -- dependancies
    -- :: directories
    libdirs {
        ROOT .. "/bin/%{cfg.system}_%{cfg.buildcfg}",
        --[[ INSERT ADDITIONAL DIRECTORIES HERE ]]
    }
    -- :: libraries
    links { WORKSPACE, --[[ INSERT ADDITIONAL LINKS HERE ]] }
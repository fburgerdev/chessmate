-- premake5.lua
ROOT = ".."
-- workspace
workspace "chessmate"
   -- startproject
   startproject "example"
   -- configuration
   configurations { "debug", "fast", "dist" }
-- library
project "chessmate"
   -- staticlib
   kind "StaticLib"
   -- cpp
   language "C++"
   cppdialect "C++latest"
   -- file
   files {
      ROOT .. "/src/**.hpp",
      ROOT .. "/src/**.cpp",
      ROOT .. "/vendor/**.hpp",
      ROOT .. "/vendor/**.cpp",
   }
   includedirs {
      ROOT .. "/src",
      ROOT .. "/vendor",
   }
   -- object
   objdir(ROOT .. "/bin")
   -- debugger
   debugger "GDB"
   -- config
   -- config :: debug
   filter "configurations:debug"
      -- symbols
      symbols "On"
      -- define
      defines { "CONFIG_DEBUG" }
      -- target
      targetdir(ROOT .. "/lib/debug")
   -- config :: fast
   filter "configurations:fast"
      -- optimize
      optimize "On"
      -- define
      defines { "CONFIG_FAST" }
      -- option
      linkoptions{ "-Ofast" }
      -- target
      targetdir(ROOT .. "/lib/fast")
   -- config :: dist
   filter "configurations:dist"
      -- optimize
      optimize "On"
      -- define
      defines { "CONFIG_DIST" }
      -- option
      linkoptions { "-Ofast" }
      -- target
      targetdir(ROOT .. "/lib/dist")
-- example
project "example"
   -- console
   kind "ConsoleApp"
   -- cpp
   language "C++"
   cppdialect "C++latest"
   -- file
   files {
      ROOT .. "/example/**.hpp",
      ROOT .. "/example/**.cpp",
   }
   includedirs {
      ROOT .. "/example",
      ROOT .. "/src",
   }
   -- link
   links { "chessmate" }
   -- object
   objdir(ROOT .. "/bin/obj")
   -- debug
   debugger "GDB"
   -- config
   filter "configurations:debug"
      -- symbols
      symbols "On"
      defines { "CONFIG_DEBUG" }
      -- target
      targetdir(ROOT .. "/bin/debug")
   filter "configurations:fast"
      -- optimize
      optimize "On"
      -- define
      defines { "CONFIG_FAST" }
      -- option
      linkoptions { "-Ofast" }
      -- target
      targetdir(ROOT .. "/bin/fast")
   filter "configurations:dist"
      -- optimize
      optimize "On"
      -- define
      defines { "CONFIG_DIST" }
      -- option
      linkoptions { "-static", "-Ofast" }
      -- target
      targetdir(ROOT .. "/bin/dist")
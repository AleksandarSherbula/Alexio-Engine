workspace "AlexioEngine"
   startproject "Engine"
   configurations { "Debug", "Release" }
   architecture "x64"

IncludeDir = {}
IncludeDir["GLFW"] = "Engine/dependecies/GLFW/include"

group "Dependencies"
   include "Engine/dependecies/GLFW"
group ""

project "Engine"
   location "Engine"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   
   targetdir "build/%{prj.name}/bin/%{cfg.system}/%{cfg.buildcfg}-%{cfg.architecture}"
   objdir    "build/%{prj.name}/intermediate/%{cfg.system}/%{cfg.buildcfg}-%{cfg.architecture}"

   pchheader "aio_pch.h"
   pchsource "%{prj.name}/src/aio_pch.cpp"
   
   files
   {
      "%{prj.name}/src/**.h",
      "%{prj.name}/src/**.cpp"
   }

   includedirs
   {
      "%{prj.name}/dependecies/spdlog/include",
      "%{prj.name}/src",
      "%{IncludeDir.GLFW}"
   }

   links
   {
      "Game",
      "GLFW",
      "opengl32.lib"
   }

   filter "system:windows"
      staticruntime "On"
      systemversion "latest"
      
      defines
      {
         "AIO_PLATFORM_WINDOWS"
      }
       
   filter "configurations:Debug"
      defines { "AIO_DEBUG" }
      symbols "On"
   
   filter "configurations:Release"
      defines { "AIO_RELEASE" }
      optimize "On"

project "Game"
   location "Game"
   kind "StaticLib"
   language "C++"
   cppdialect "C++17"
   
   targetdir "build/%{prj.name}/bin/%{cfg.system}/%{cfg.buildcfg}-%{cfg.architecture}"
   objdir    "build/%{prj.name}/intermediate/%{cfg.system}/%{cfg.buildcfg}-%{cfg.architecture}"
   
   files
   {
      "%{prj.name}/src/**.h",
      "%{prj.name}/src/**.cpp"
   }

   includedirs
   {
      "Engine/dependecies/spdlog/include",
      "Engine/src"
   }

   filter "system:windows"
      staticruntime "On"
      systemversion "latest"
      
      defines
      {
         "AIO_PLATFORM_WINDOWS"
      }
       
   filter "configurations:Debug"
      defines { "AIO_DEBUG" }
      symbols "On"
   
   filter "configurations:Release"
      defines { "AIO_RELEASE" }
      optimize "On"
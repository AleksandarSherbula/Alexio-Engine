workspace "AlexioEngine"
   startproject "Engine"
   configurations { "Debug", "Release" }
   architecture "x64"

IncludeDir = {}
IncludeDir["GLFW"]  = "Engine/dependecies/GLFW/include"
IncludeDir["GLAD"]  = "Engine/dependecies/GLAD/include"
IncludeDir["ImGUI"] = "Engine/dependecies/imgui"
IncludeDir["GLM"]   = "Engine/dependecies/glm"

group "Dependencies"
   include "Engine/dependecies/GLFW"
   include "Engine/dependecies/GLAD"
   include "Engine/dependecies/imgui"
group ""

project "Engine"
   location "Engine"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   staticruntime "On"
   
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
      "%{IncludeDir.GLFW}",
      "%{IncludeDir.GLAD}",
      "%{IncludeDir.ImGUI}",
      "%{IncludeDir.GLM}"
   }

   links
   {
      "Game",
      "GLFW",
      "GLAD",
      "ImGUI",
      "opengl32.lib"
   }

   filter "system:windows"      
      systemversion "latest"
      
      defines
      {
         "AIO_PLATFORM_WINDOWS"
      }

      links
      {
         "DXGI.lib",
         "D3D11.lib"
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
   staticruntime "On"
   
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

   filter "configurations:Debug"
      defines { "AIO_DEBUG" }
      symbols "On"
   
   filter "configurations:Release"
      defines { "AIO_RELEASE" }
      optimize "On"

   filter "system:windows"
      staticruntime "On"
      systemversion "latest"
      
      defines
      {
         "AIO_PLATFORM_WINDOWS"
      }  
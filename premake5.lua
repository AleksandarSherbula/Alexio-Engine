workspace "AlexioEngine"
   startproject "Editor"
   configurations { "Debug", "Release" }
   architecture "x64"

IncludeDir = {}
IncludeDir["GLFW"]  = "Engine/dependencies/GLFW/include"
IncludeDir["GLAD"]  = "Engine/dependencies/GLAD/include"
IncludeDir["ImGUI"] = "Engine/dependencies/imgui"
IncludeDir["GLM"]   = "Engine/dependencies/glm"
IncludeDir["Entt"]  = "Engine/dependencies/entt/include"

group "Dependencies"
   include "Engine/dependencies/GLFW"
   include "Engine/dependencies/GLAD"
   include "Engine/dependencies/imgui"
group ""

project "Engine"
   location "Engine"
   kind "StaticLib"
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
      "%{prj.name}/src/**.cpp",
      "%{prj.name}/dependencies/stb_image/stb_image.h",
      "%{prj.name}/dependencies/stb_image/stb_image.cpp",
   }

   defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

   includedirs
   {
      "%{prj.name}/src",
      "%{prj.name}/dependencies/spdlog/include",
      "%{prj.name}/dependencies/",
      "%{IncludeDir.GLFW}",
      "%{IncludeDir.GLAD}",
      "%{IncludeDir.ImGUI}",
      "%{IncludeDir.GLM}",
      "%{IncludeDir.Entt}"
   }

   links
   {
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

project "Editor"
   location "Editor"
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
      "Engine/dependencies/spdlog/include",
      "Engine/src",
      "Engine/dependencies/",
      "%{IncludeDir.ImGUI}",
      "%{IncludeDir.GLM}"
   }

   links
   {
      "Engine"
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
      
   project "Sandbox"
      location "Sandbox"
      kind "ConsoleApp"
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
         "Engine/dependencies/spdlog/include",
         "Engine/src",
         "Engine/dependencies/",
         "%{IncludeDir.ImGUI}",
         "%{IncludeDir.GLM}"
      }
   
      links
      {
         "Engine"
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
workspace "Keela"
	architecture "x64"
	startproject "KitchenSink"

	configurations
	{	
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Keela/vender/GLFW/include"
IncludeDir["Glad"] = "Keela/vender/Glad/include"
IncludeDir["ImGui"] = "Keela/vender/imgui"

include "Keela/vender/GLFW"
include "Keela/vender/Glad"
include "Keela/vender/imgui"

group "Dependencies"
	include "Keela/vender/GLFW"
	include "Keela/vender/Glad"
	include "Keela/vender/imgui"
group ""

project "Keela"
	location "Keela"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "keepch.h"
	pchsource "Keela/src/keepch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vender/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		"dwmapi.lib"
	}

	filter "system:windows"
	cppdialect "C++17"
	staticruntime "On"
	systemversion "latest"

	defines
	{
		"KEE_PLATFORM_WINDOWS",
		"KEE_BUILD_DLL",
		"GLFW_INCLUDE_NONE"

	}

	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} \"../bin/" ..outputdir .. "/KitchenSink/\"")
	}

	filter "configurations:Debug"
		defines "KEE_DEBUG"
		staticruntime "off"
		runtime"Debug"
		symbols "On"

	filter "configurations:Release"
		defines "KEE_RELEASE"
		staticruntime "off"
		runtime"Release"
		optimize "On"

	filter "configurations:Dist"
		defines "KEE_DIST"
		staticruntime "off"
		runtime"Release"
		optimize "On"


project "KitchenSink"
	location "KitchenSink"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Keela/vender/spdlog/include",
		"Keela/src"
	}

	links
	{
		"Keela"
	}

	filter "system:windows"
	cppdialect "C++17"
	staticruntime "On"
	systemversion "latest"

	defines
	{
		"KEE_PLATFORM_WINDOWS"

	}

	filter "configurations:Debug"
		defines "KEE_DEBUG"
		staticruntime "off"
		runtime"Debug"
		symbols "On"

	filter "configurations:Release"
		defines "KEE_RELEASE"
		staticruntime "off"
		runtime"Release"
		optimize "On"

	filter "configurations:Dist"
		defines "KEE_DIST"
		staticruntime "off"
		runtime"Release"
		optimize "On"

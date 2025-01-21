
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
IncludeDir["GLFW"] = "Keela/vender/glfw/include"
IncludeDir["Glad"] = "Keela/vender/Glad/include"
IncludeDir["ImGui"] = "Keela/vender/imgui"
IncludeDir["ImPlot"] = "Keela/vender/ImPlot"
IncludeDir["glm"] = "Keela/vender/glm"
IncludeDir["stb"] = "Keela/vender/stb"
IncludeDir["BrassMono"] = "Keela/vender/brass-mono-font"


include "Keela/vender/glfw"
include "Keela/vender/Glad"
include "Keela/vender/imgui"
include "Keela/vender/ImPlot"


group "Dependencies"
	include "Keela/vender/glfw"
	include "Keela/vender/Glad"
	include "Keela/vender/imgui"
	include "Keela/vender/ImPlot"
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
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vender/glm/glm/**.hpp",
		"%{prj.name}/vender/glm/glm/**.inl",
		"%{prj.name}/vender/brass-mono-font/**.ttf",
		"%{prj.name}/vender/stb/stb_image.h"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vender/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImPlot}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.BrassMono}"
	}

	links{
		"GLFW",
		"Glad",
		"ImGui",
		"ImPlot",
		"opengl32.lib",
		"dwmapi.lib"
	}

	libdirs
	{
		"%{prj.name}/bin/" .. outputdir .. "/ImPlot",
		"../Basler/pylon7/Applications/x64/lib/pylonviewer/plugins/"
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
		exceptionhandling "On"

	filter "configurations:Release"
		defines "KEE_RELEASE"
		staticruntime "off"
		runtime"Release"
		optimize "On"
		exceptionhandling "On"

	filter "configurations:Dist"
		defines "KEE_DIST"
		staticruntime "off"
		runtime"Release"
		optimize "On"
		exceptionhandling "On"


    -- Custom configuration for PCH usage
    filter "configurations:UsePCH"
        defines "USE_PCH"      -- Define USE_PCH flag
        pchheader "keepch.h"      -- Specify the precompiled header
        pchsource "%{prj.name}/vender/ImPlot/implot.cpp"  -- Specify the source file for the PCH

		
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
		"Keela/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.BrassMono}",
		"%{IncludeDir.ImPlot}",
		"Keela/vender/"
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

workspace "Keela"
	architecture "x64"

	configurations
	{	
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Keela"
	location "Keela"
	kind "SharedLib"
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
		"%{prj.name}/vender/spdlog/include"
	}


	filter "system:windows"
	cppdialect "C++17"
	staticruntime "On"
	systemversion "latest"

	defines
	{
		"KEE_PLATFORM_WINDOWS",
		"KEE_BUILD_DLL"

	}

	postbuildcommands
	{
	 ("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" ..outputdir .. "/KitchenSink")
	}

	filter "configurations:Debug"
		defines "KEE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "KEE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "KEE_DIST"
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
		symbols "On"

	filter "configurations:Release"
		defines "KEE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "KEE_DIST"
		optimize "On"

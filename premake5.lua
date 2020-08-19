workspace "Mandelbrot"
	architecture "x64"
	configurations {"Debug", "Release"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}"

-- Jolt Core --
group "Jolt"

include "Jolt/Jolt"
include "Jolt/Jolt/vendor/GLFW"
include "Jolt/Jolt/vendor/ImGui"
include "Jolt/Jolt/vendor/stb_image"
include "Jolt/Jolt/vendor/GLAD"


group ""
project "Mandelbrot"
	location ""
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17" 

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"src/**.h",
		"src/**.cpp",
	}

	includedirs 
	{
		"Jolt/Jolt/src",
		
		"Jolt/Jolt/vendor/GLFW/include",
		"Jolt/Jolt/vendor/GLM",
		"Jolt/Jolt/vendor/imgui",
		"Jolt/Jolt/vendor/GLAD/include",
		"Jolt/Jolt/vendor/spdlog/include"
	}

	links
	{
		"Jolt",
		"stb_image",
		"ImGui",
		"GLFW",
		"Glad",
	}

	filter "configurations:Debug"
		defines { "DEBUG", "JOLT_PROFILE" }
		symbols "On"

	filter "configurations:Release"
		defines { "RELEASE" }
		optimize "On"

	filter "system:linux"
		links { "dl", "pthread" }
		postbuildcommands 
		{
			"@echo cp %{wks.location}/bin/" .. outputdir .. "/%{prj.name}/%{prj.name} %{prj.location}/%{prj.name}.out",
			"@cp %{wks.location}/bin/" .. outputdir .. "/%{prj.name}/%{prj.name} %{prj.location}/%{prj.name}.out"
		}
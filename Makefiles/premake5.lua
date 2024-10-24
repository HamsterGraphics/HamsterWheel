dofile("utils.lua")

local PlatformNameToGfxBackendName = {}
PlatformNameToGfxBackendName["Windows"] = "D3D12"
PlatformNameToGfxBackendName["MacOS"] = "Metal"

--------------------------------------------------------------
-- Path
--------------------------------------------------------------
HG_MODULE_SHARED = os.getenv("HG_MODULE_SHARED") or false
HG_GFX_DYNAMIC_DLL = os.getenv("HG_GFX_DYNAMIC_DLL") or true
HG_GFX_ENABLE_DEBUG = os.getenv("HG_GFX_ENABLE_DEBUG") or true
PlatformName = os.getenv("HG_PLATFORM_NAME")
GfxBackendName = PlatformNameToGfxBackendName[PlatformName]
CurrentWorkingPath = os.getcwd()
MakeFileRelativePath = "Makefiles"
RootPath = string.sub(CurrentWorkingPath, 0, string.len(CurrentWorkingPath) - string.len(MakeFileRelativePath) - 1)
MakefilePath = path.join(RootPath, MakeFileRelativePath)
BuildOutputPath = path.join(RootPath, "Build")
SourcePath = path.join(RootPath, "Source")
ExamplesPath = path.join(RootPath, "Examples")
PluginsPath = path.join(RootPath, "Plugins")
--------------------------------------------------------------
-- Dump Info
--------------------------------------------------------------
Log.Info("-----------------------Path---------------------------")
Dump.PrintVar("HG_MODULE_SHARED", HG_MODULE_SHARED)
Dump.PrintVar("HG_GFX_DYNAMIC_DLL", HG_GFX_DYNAMIC_DLL)
Dump.PrintVar("HG_GFX_ENABLE_DEBUG", HG_GFX_ENABLE_DEBUG)
Dump.PrintVar("PlatformName", PlatformName)
Dump.PrintVar("GfxBackendName", GfxBackendName)
Dump.PrintVar("CurrentWorkingPath", CurrentWorkingPath)
Dump.PrintVar("RootPath", RootPath)
Dump.PrintVar("MakefilePath", MakefilePath)
Dump.PrintVar("BuildOutputPath", BuildOutputPath)
Dump.PrintVar("SourcePath", SourcePath)
Dump.PrintVar("ExamplesPath", ExamplesPath)
Dump.PrintVar("PluginsPath", PluginsPath)
Log.Info("------------------------------------------------------")

--------------------------------------------------------------
-- Workspace
--------------------------------------------------------------
workspace("HamsterWheel")
	location(RootPath)
	startproject("00_Triangle")

	filter { "system:windows" }
		architecture("x86_64")
	filter { "system:macosx" }
		architecture("universal")
	filter {}
	systemversion("latest")

	configurations { "Debug", "Release" }
	filter { "configurations:Debug" }
		defines { "_DEBUG" }
		symbols("On")
		optimize("Off")
	filter { "configurations:Release" }
		defines { "NDEBUG" }
		symbols("On")
		optimize("Full")
	filter {}
	

--------------------------------------------------------------	
-- Utility Project
--------------------------------------------------------------
group("Build")
dofile("make.lua")
group("")

group("Modules")
dofile("core.lua")
dofile("os.lua")
dofile("graphics.lua")
dofile("scene.lua")
group("")

dofile("plugins.lua")
dofile("application.lua")
dofile("examples.lua")
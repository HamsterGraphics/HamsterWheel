dofile("utils.lua")

--------------------------------------------------------------
-- Path
--------------------------------------------------------------
BuildModuleAsDll = os.getenv("HG_MODULE_SHARED") or false
PlatformName = os.getenv("HG_PLATFORM_NAME") or "Windows"
BackendName = "D3D12"
CurrentWorkingPath = os.getcwd()
MakeFileRelativePath = "Makefiles"
RootPath = string.sub(CurrentWorkingPath, 0, string.len(CurrentWorkingPath) - string.len(MakeFileRelativePath) - 1)
MakefilePath = path.join(RootPath, MakeFileRelativePath)
BuildOutputPath = path.join(RootPath, "Build")
SourcePath = path.join(RootPath, "Source")
ExamplesPath = path.join(RootPath, "Examples")

--------------------------------------------------------------
-- Dump Info
--------------------------------------------------------------
Log.Info("-----------------------Path---------------------------")
Dump.PrintVar("BuildModuleAsDll", BuildModuleAsDll)
Dump.PrintVar("PlatformName", PlatformName)
Dump.PrintVar("CurrentWorkingPath", CurrentWorkingPath)
Dump.PrintVar("RootPath", RootPath)
Dump.PrintVar("MakefilePath", MakefilePath)
Dump.PrintVar("BuildOutputPath", BuildOutputPath)
Dump.PrintVar("SourcePath", SourcePath)
Dump.PrintVar("ExamplesPath", ExamplesPath)
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

dofile("application.lua")
dofile("examples.lua")
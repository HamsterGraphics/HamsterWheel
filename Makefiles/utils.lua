--------------------------------------------------------------
-- Dump
--------------------------------------------------------------
Dump = {}
Dump.PrintVar = function(name, value)
	term.pushColor(term.cyan)
	print(name.." : "..tostring(value))
	term.popColor()
end

--------------------------------------------------------------
-- Log
--------------------------------------------------------------
Log = {}
Log.Info = function(msg)
	term.pushColor(term.green)
	print("[Info] "..msg)
	term.popColor()
end

Log.Warning = function(msg)
	term.pushColor(term.yellow)
	print("[Warning] "..msg)
	term.popColor()
end

Log.Error = function(msg)
	term.pushColor(term.red)
	print("[Error] "..msg)
	term.popColor()
end

--------------------------------------------------------------
-- Project
--------------------------------------------------------------
Project = {}
Project.CLanguage = function()
	language("C")
	cdialect("C99")
end

Project.CppLanguage = function()
	language("C++")
	cppdialect("C++20")
end

Project.GetIntermediateDirectory = function()
	return "%{prj.location}/Intermediate/%{cfg.buildcfg}_%{cfg.architecture}"
end

Project.GetBinariesDirectory = function()
	return "%{prj.location}/Binaries/%{cfg.buildcfg}_%{cfg.architecture}"
end

Project.Location = function(outputPath)
	location(outputPath)
	objdir(Project.GetIntermediateDirectory())
	targetdir(Project.GetBinariesDirectory())
end

Project.StaticRuntime = function(flag)
	-- Dll contains its own copy of the C Runtime and heap manager in /MT[d]. 
	staticruntime(flag)
	filter { "configurations:Debug" }
		runtime("Debug")
	filter { "configurations:Release" }
		runtime("Release")
	filter {}
end

Project.CommonSettings = function()
	-- Disable these options can reduce the size of compiled binaries.
	justmycode("Off")
	editAndContinue("Off")
	exceptionhandling("Off")
	rtti("Off")	

	-- Strict compile.
	warnings("Default")
	externalwarnings("Off")

	flags {
		"MultiProcessorCompile", -- speed up compile
		"FatalWarnings", -- treat warnings as errors
	}
end
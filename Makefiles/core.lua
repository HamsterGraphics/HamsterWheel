project("Core")
	if HG_MODULE_SHARED then
		kind("SharedLib")
		defines { "HG_CORE_EXPORT", "HG_MODULE_SHARED" }
	else
		kind("StaticLib")
	end
	
	Project.CppLanguage()
	Project.Location(BuildOutputPath)
	Project.StaticRuntime("on")
	Project.CommonSettings()

	location(path.join(RootPath, "Build"))
	
	includedirs {
		path.join(SourcePath, "Core/Public"),
	}

	files {
		path.join(SourcePath, "Core/**.*")
	}

	vpaths {
		["Public/*"] = path.join(SourcePath, "Core/Public/*/**.*"),
		["Private/*"] = path.join(SourcePath, "Core/Private/*/**.*")
	}
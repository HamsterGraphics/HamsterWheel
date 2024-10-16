project("Core")
	kind("StaticLib")
	
	Project.CppLanguage()
	Project.Location(BuildOutputPath)
	Project.StaticRuntime("on")

	location(path.join(RootPath, "Build"))
	
	defines {
		"HG_EXPORT"
	}

	includedirs {
		path.join(SourcePath, "Core"),
	}

	files {
		path.join(SourcePath, "Core/**.*")
	}
project("Application")
	kind("StaticLib")

	Project.CppLanguage()
	Project.Location(BuildOutputPath)
	Project.StaticRuntime("on")
	Project.CommonSettings()

	dependson {
		"Core", "OS"
	}

	includedirs {
		path.join(SourcePath, "Core/Public"),
		path.join(SourcePath, "OS/Public"),
		path.join(SourcePath, "Application/Public"),
	}

	files {
		path.join(SourcePath, "Application/Public/**.*"),
		path.join(SourcePath, "Application/Private/"..PlatformName.."/**.*")
	}

	vpaths {
		["Public"] = path.join(SourcePath, "Application/Public/**.*"),
		["Private"] = path.join(SourcePath, "Application/Private/"..PlatformName.."/**.*")
	}

	links {
		"Core", "OS"
	}
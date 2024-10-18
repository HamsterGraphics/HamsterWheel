project("Application")
	kind("StaticLib")

	Project.CppLanguage()
	Project.Location(BuildOutputPath)
	Project.StaticRuntime("on")
	dependson {
		"OS"
	}

	defines {
		--"HG_APP_EXPORT"
	}

	includedirs {
		path.join(SourcePath, "Core"),
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
		"OS"
	}

	Project.CommonSettings()
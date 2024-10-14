project("OS")
	kind("StaticLib")

	Project.CppLanguage()
	Project.Location(BuildOutputPath)
	Project.StaticRuntime("on")

	defines {
	}

	includedirs {
		path.join(SourcePath, "Core"),
		path.join(SourcePath, "OS/Public"),
	}

	files {
		path.join(SourcePath, "OS/Public/**.*"),
		path.join(SourcePath, "OS/Private/"..PlatformName.."/**.*")
	}

	vpaths {
		["Public"] = path.join(SourcePath, "OS/Public/**.*"),
		["Private"] = path.join(SourcePath, "OS/Private/"..PlatformName.."/**.*")
	}

	Project.CommonSettings()
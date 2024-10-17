project("Graphics")
	kind("StaticLib")

	Project.CppLanguage()
	Project.Location(BuildOutputPath)
	Project.StaticRuntime("on")

	dependson {
		"OS"
	}

	defines {
		"HG_EXPORT"
	}

	includedirs {
		path.join(SourcePath, "Core"),
		path.join(SourcePath, "OS/Public"),
		path.join(SourcePath, "Graphics/Public"),
	}

	files {
		path.join(SourcePath, "Graphics/Public/**.*"),
		path.join(SourcePath, "Graphics/Private/"..BackendName.."/**.*")
	}

	vpaths {
		["Public"] = path.join(SourcePath, "Graphics/Public/**.*"),
		["Private"] = path.join(SourcePath, "Graphics/Private/"..BackendName.."/**.*")
	}

	links {
		"OS"
	}

	Project.CommonSettings()
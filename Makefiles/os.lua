project("OS")
	if BuildModuleAsDll then
		kind("SharedLib")
		defines { "HG_OS_EXPORT", "HG_MODULE_SHARED" }
	else
		kind("StaticLib")
	end

	Project.CppLanguage()
	Project.Location(BuildOutputPath)
	Project.StaticRuntime("on")

	includedirs {
		path.join(SourcePath, "Core/Public"),
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

	links {
	}

	Project.CommonSettings()
project("Graphics")
	if BuildModuleAsDll then
		kind("SharedLib")
		defines { "HG_GFX_EXPORT", "HG_MODULE_SHARED" }
	else
		kind("StaticLib")
	end

	Project.CppLanguage()
	Project.Location(BuildOutputPath)
	Project.StaticRuntime("on")

	includedirs {
		path.join(SourcePath, "Core/Public"),
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
	}

	Project.CommonSettings()
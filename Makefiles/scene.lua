project("Scene")
	if BuildModuleAsDll then
		kind("SharedLib")
		defines { "HG_SCENE_EXPORT", "HG_MODULE_SHARED" }
	else
		kind("StaticLib")
	end

	Project.CppLanguage()
	Project.Location(BuildOutputPath)
	Project.StaticRuntime("on")

	includedirs {
		path.join(SourcePath, "Core/Public"),
		path.join(SourcePath, "Scene/Public"),
	}

	files {
		path.join(SourcePath, "Scene/Public/**.*"),
	}

	vpaths {
		["Public"] = path.join(SourcePath, "Scene/Public/**.*"),
		["Private"] = path.join(SourcePath, "Scene/Private/**.*")
	}

	Project.CommonSettings()
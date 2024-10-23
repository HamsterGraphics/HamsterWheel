project("Scene")
	if HG_MODULE_SHARED then
		kind("SharedLib")
		defines { "HG_SCENE_EXPORT", "HG_MODULE_SHARED" }
	else
		kind("StaticLib")
	end

	Project.CppLanguage()
	Project.Location(BuildOutputPath)
	Project.StaticRuntime("on")
	Project.CommonSettings()

	includedirs {
		path.join(SourcePath, "Core/Public"),
		path.join(SourcePath, "Scene/Public"),
	}

	files {
		path.join(SourcePath, "Scene/**.*"),
	}

	vpaths {
		["Public/*"] = path.join(SourcePath, "Scene/Public/*/**.*"),
		["Private/*"] = path.join(SourcePath, "Scene/Private/*/**.*")
	}
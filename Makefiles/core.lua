project("Core")
	if BuildModuleAsDll then
		kind("SharedLib")
		defines { "HG_CORE_EXPORT", "HG_MODULE_SHARED" }
	else
		kind("StaticLib")
	end
	
	Project.CppLanguage()
	Project.Location(BuildOutputPath)
	Project.StaticRuntime("on")

	location(path.join(RootPath, "Build"))
	
<<<<<<< HEAD
=======
	includedirs {
		path.join(SourcePath, "Core/Public"),
	}

>>>>>>> b8fbc6bbccaca79626a3ea8bfd7adff3ad6d3c6d
	files {
		path.join(SourcePath, "Core/**.*")
	}

	vpaths {
		["Public/*"] = path.join(SourcePath, "Core/Public/*/**.*"),
		["Private"] = path.join(SourcePath, "Core/Private/**.*")
	}
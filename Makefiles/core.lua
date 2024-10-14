project("Core")
	kind("Utility")
	
	location(path.join(RootPath, "Build/Misc"))
	
	files {
		path.join(SourcePath, "Core/**.*")
	}
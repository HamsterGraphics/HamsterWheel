project("Make")
	kind("Utility")
	location(path.join(RootPath, "Build/Misc"))
	files {
		path.join(MakefileSourcePath, "**.lua")
	}

	filter { "system:windows", "action:vs2022" }
		prebuildcommands {
			"cd "..RootPath,
			"Generate_Windows.bat",
		}
	filter {}
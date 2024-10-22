project("Make")
	kind("Utility")
	location(path.join(RootPath, "Build/Misc"))
	files {
		path.join(MakefileSourcePath, "**.lua")
	}

	filter { "system:windows" }
		prebuildcommands {
			"cd "..RootPath,
			"Generate_Windows.bat",
		}
	filter { "system:macosx" }
		prebuildcommands {
			"cd "..RootPath,
			"Generate_MacOS.bat",
		}
	filter {}
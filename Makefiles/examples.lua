local function MakeExample(exampleFolder)
	local exampleName = path.getbasename(exampleFolder)
	project(exampleName)
		kind("ConsoleApp")
		dependson { "OS" }

		Project.CppLanguage()
		Project.Location(BuildOutputPath)
		Project.StaticRuntime("on")
		Project.CommonSettings()

		files {
			path.join(exampleFolder, "**.*")
		}

		includedirs {
			path.join(SourcePath, "Core"),
			path.join(SourcePath, "OS/Public"),
		}

		links {
			"OS"
		}
end

group("Examples")
local exampleFolders = os.matchdirs(path.join(ExamplesPath, "*"))
for _, exampleFolder in pairs(exampleFolders) do
	MakeExample(exampleFolder)
end
group("")
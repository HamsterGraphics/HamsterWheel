local function MakeExample(exampleFolder)
	local exampleName = path.getbasename(exampleFolder)
	project(exampleName)
		kind("ConsoleApp")
		dependson { "Core", "Application", "Graphics" }

		Project.CppLanguage()
		Project.Location(BuildOutputPath)
		Project.StaticRuntime("on")
		Project.CommonSettings()

		defines {
			"HG_GFX_BACKEND_"..string.upper(GfxBackendName)
		}

		if HG_GFX_ENABLE_DEBUG then
			defines { "HG_GFX_ENABLE_DEBUG" }
		end

		files {
			path.join(exampleFolder, "**.*")
		}

		includedirs {
			path.join(SourcePath, "Core/Public"),
			path.join(SourcePath, "OS/Public"),
			path.join(SourcePath, "Graphics/Public"),
			path.join(SourcePath, "Graphics/Public/"..GfxBackendName),
			path.join(SourcePath, "Application/Public"),
		}

		links {
			"Core", "Application", "Graphics"
		}
end

group("Examples")
local exampleFolders = os.matchdirs(path.join(ExamplesPath, "*"))
for _, exampleFolder in pairs(exampleFolders) do
	MakeExample(exampleFolder)
end
group("")
local function MakePlugin(pluginFolder)
	local pluginName = path.getbasename(pluginFolder)
	project(pluginName)
		kind("SharedLib")
		--dependson { "Core" }

		Project.CppLanguage()
		Project.Location(BuildOutputPath)
		Project.StaticRuntime("on")
		Project.CommonSettings()

		files {
			path.join(pluginFolder, "**.*")
		}

		includedirs {
			path.join(SourcePath, "Core/Public"),
		}

		links {
			--"Core"
		}
end

group("Plugins")
local pluginFolders = os.matchdirs(path.join(PluginsPath, "*"))
for _, pluginFolder in pairs(pluginFolders) do
	MakePlugin(pluginFolder)
end
group("")
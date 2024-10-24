local GfxBackendMakeCallbacks = {}
GfxBackendMakeCallbacks["D3D12"] = function()
	if HG_GFX_DYNAMIC_DLL then
		defines { "HG_GFX_DYNAMIC_API" }
	else
		links {
			"d3d12", "dxgi", "d3dcompiler"
		}
	end
end

project("Graphics")
	if HG_MODULE_SHARED then
		kind("SharedLib")
		defines { "HG_GFX_EXPORT", "HG_MODULE_SHARED" }
	else
		kind("StaticLib")
	end

	dependson { "Core", "OS" }

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

	includedirs {
		path.join(SourcePath, "Core/Public"),
		path.join(SourcePath, "OS/Public"),
		path.join(SourcePath, "Graphics/Public"),
		path.join(SourcePath, "Graphics/ThirdParty"),
	}

	files {
		path.join(SourcePath, "Graphics/Public/**.*"),
		path.join(SourcePath, "Graphics/Private/"..GfxBackendName.."/**.*")
	}

	vpaths {
		["Public"] = path.join(SourcePath, "Graphics/Public/**.*"),
		["Private"] = path.join(SourcePath, "Graphics/Private/"..GfxBackendName.."/**.*")
	}

	links {
		"Core", "OS"
	}

	local GfxCallback = GfxBackendMakeCallbacks[GfxBackendName]
	if GfxCallback then
		GfxCallback()
	end
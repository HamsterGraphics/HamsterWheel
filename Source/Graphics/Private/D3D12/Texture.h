/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "IGraphics.h"

namespace hg
{

typedef struct TextureDesc
{
	uint32 Width;
	uint32 Height;
	uint32 Depth;
	uint32 Mips;
	uint32 ArraySize;
	DXGI_FORMAT Format;
} TextureDesc;

class Texture
{
public:
	Texture() = default;
	explicit Texture(ID3D12Resource* pResource);
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture(Texture&&) = delete;
	Texture& operator=(Texture&&) = delete;
	~Texture() = default;

	TextureDesc& GetDesc() { return m_desc; }
	const TextureDesc& GetDesc() const { return m_desc; }
	ID3D12Resource* GetHandle() const { return m_resource; }

private:
	TextureDesc m_desc;
	RefCountPtr<ID3D12Resource> m_resource;
	uint32 m_srvDescriptorID;
};

}
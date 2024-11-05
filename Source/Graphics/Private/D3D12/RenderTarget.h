/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "IGraphics.h"

#include "Texture.h"

namespace hg
{

class RenderTarget
{
public:
	RenderTarget() = delete;
	explicit RenderTarget(ID3D12Device* pDevice, D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle);
	RenderTarget(const RenderTarget&) = delete;
	RenderTarget& operator=(const RenderTarget&) = delete;
	RenderTarget(RenderTarget&&) = delete;
	RenderTarget& operator=(RenderTarget&&) = delete;
	~RenderTarget() = default;

	ID3D12Resource* GetHandle() const { return m_texture.GetHandle(); }

private:
	Texture m_texture;
	D3D12_CPU_DESCRIPTOR_HANDLE m_rtv;
};

}
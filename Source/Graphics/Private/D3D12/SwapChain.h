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

class RenderTarget;

class SwapChain
{
public:
	SwapChain() = delete;
	explicit SwapChain(IDXGIFactory6* pFactory, const SwapChainDesc& desc);
	SwapChain(const SwapChain&) = delete;
	SwapChain& operator=(const SwapChain&) = delete;
	SwapChain(SwapChain&&) = delete;
	SwapChain& operator=(SwapChain&&) = delete;
	~SwapChain() = default;

	void BeginFrame();
	void EndFrame();
	uint32 GetBackBufferIndex() const { return m_bufferIndex; }
	IDXGISwapChain3* GetHandle() const { return m_swapChain; }
	void CreateBackBufferRTV(ID3D12Device* pDevice, uint32 index, D3D12_CPU_DESCRIPTOR_HANDLE rtv);

private:
	RefCountPtr<IDXGISwapChain3> m_swapChain;
	RenderTarget* m_renderTargets[MAX_FRAME_COUNT];
	uint32 m_bufferIndex;
	uint32 m_bufferWidth;
	uint32 m_bufferHeight;
};

}
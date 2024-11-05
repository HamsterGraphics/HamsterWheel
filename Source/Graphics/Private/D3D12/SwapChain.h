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

	IDXGISwapChain3* GetHandle() const { return m_swapChain; }

private:
	RefCountPtr<IDXGISwapChain3> m_swapChain;
	uint32 m_bufferIndex;
	uint32 m_bufferWidth;
	uint32 m_bufferHeight;
};

}
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

class Fence
{
public:
	Fence() = delete;
	explicit Fence(ID3D12Device* pDevice);
	Fence(const Fence&) = delete;
	Fence& operator=(const Fence&) = delete;
	Fence(Fence&&) = delete;
	Fence& operator=(Fence&&) = delete;
	~Fence();

private:
	RefCountPtr<ID3D12Fence> m_fence;
	HANDLE m_completeEvent;
	uint64 m_fenceValue;
};

}
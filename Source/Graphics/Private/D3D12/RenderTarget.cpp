/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "RenderTarget.h"

namespace hg
{

RenderTarget::RenderTarget(ID3D12Device* pDevice, D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle) :
	m_rtv(rtvHandle)
{
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = { };
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Format = m_texture.GetDesc().Format;
    rtvDesc.Texture2D.MipSlice = 0;
    rtvDesc.Texture2D.PlaneSlice = 0;
    pDevice->CreateRenderTargetView(m_texture.GetHandle(), &rtvDesc, m_rtv);
}

}
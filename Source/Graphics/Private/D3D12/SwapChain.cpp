/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "SwapChain.h"

#include "CommandQueue.h"

namespace hg
{

SwapChain::SwapChain(IDXGIFactory6* pFactory, const SwapChainDesc& desc)
{
    BOOL allowTearing = FALSE;
    D3D12_VERIFY(pFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing)));

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc = {};
    fullScreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    fullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    fullScreenDesc.Windowed = TRUE;

    RefCountPtr<IDXGISwapChain1> pSwapChain1;
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width = desc.BufferWidth;
    swapChainDesc.Height = desc.BufferHeight;
    swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.Stereo = false;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = desc.BufferCount;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainDesc.Flags = allowTearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
    D3D12_VERIFY(pFactory->CreateSwapChainForHwnd(desc.PresentQueue->GetHandle(), (HWND)desc.NativeWindowHandle,
        &swapChainDesc, &fullScreenDesc, NULL, &pSwapChain1));
    D3D12_VERIFY(pFactory->MakeWindowAssociation((HWND)desc.NativeWindowHandle, DXGI_MWA_NO_ALT_ENTER));
    D3D12_VERIFY(D3D12Cast(pSwapChain1, &m_swapChain));


}

}
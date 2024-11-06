/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "Texture.h"

namespace hg
{

Texture::Texture(ID3D12Resource* pResource) :
	m_resource(pResource)
{
}

}
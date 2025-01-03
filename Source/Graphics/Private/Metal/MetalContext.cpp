﻿/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "IGraphics.h"

bool Graphics_Init(const GraphicsContextCreateInfo& createInfo, GraphicsContext* pContext)
{
	UNUSED(createInfo);
	UNUSED(pContext);
	return true;
}

void Graphics_Shutdown(GraphicsContext* pContext)
{
	UNUSED(pContext);
}
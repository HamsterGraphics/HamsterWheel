/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "OSConfig.h"

 ///////////////////////////////////////////////////////
 // Windows Management
 ///////////////////////////////////////////////////////
typedef struct WindowInfo
{
	const char* Name;
	void* Handle;
	void* ParentHandle;
	Rect WindowRect;

	uint32 Hide : 1;
	uint32 Maxmized : 1;
	uint32 Minimized : 1;
	uint32 FullScreen : 1;
	uint32 Borderless : 1;
} WindowInfo;

C_ABI HG_OS_API bool HG_CALLDECL Window_Init(void* pWindowProcessFunc);
C_ABI HG_OS_API bool HG_CALLDECL Window_HandleMessages();
C_ABI HG_OS_API void HG_CALLDECL Window_Create(WindowInfo* pWindowDesc);
C_ABI HG_OS_API void HG_CALLDECL Window_Destroy(WindowInfo* pWindowDesc);

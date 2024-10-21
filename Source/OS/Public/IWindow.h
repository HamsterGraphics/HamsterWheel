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

	bool Hide : 1;
	bool Maxmized : 1;
	bool Minimized : 1;
	bool FullScreen : 1;
	bool Borderless : 1;
	bool Padding : 3;
} WindowInfo;

C_ABI HG_OS_API bool HG_CALLDECL Window_Init(void* pWindowProcessFunc);
C_ABI HG_OS_API bool HG_CALLDECL Window_HandleMessages();
C_ABI HG_OS_API void HG_CALLDECL Window_Create(WindowInfo* pWindowDesc);
C_ABI HG_OS_API void HG_CALLDECL Window_Destroy(WindowInfo* pWindowDesc);

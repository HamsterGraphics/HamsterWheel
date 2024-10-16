/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include <Config.h>

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

bool Window_Init();
bool Window_HandleMessages();
HG_API void Window_Create(WindowInfo* pWindowDesc);
HG_API void Window_Destroy(WindowInfo* pWindowDesc);
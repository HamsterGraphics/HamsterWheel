/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include <Config.h>

typedef struct Rect
{
	int32 X;
	int32 Y;
	int32 Width;
	int32 Height;
} Rect;

typedef struct WindowDesc
{
	const char* Name;
	void* Handle;
	Rect WindowRect;

	bool Hide : 1;
	bool Maxmized : 1;
	bool Minimized : 1;
	bool FullScreen : 1;
	bool Borderless : 1;
} WindowDesc;

typedef struct MonitorDesc
{
	Rect MonitorRect;
} MonitorDesc;


///////////////////////////////////////////////////////
// Windows Management
///////////////////////////////////////////////////////
bool Window_InitSystem();
bool Window_HandleMessages();
HG_API void Window_Create(WindowDesc* pWindowDesc);
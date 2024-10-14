/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include <Math/Rect.h>

typedef struct WindowDesc
{
	const char* Title;
	void* Handle;
	Rect WindowRect;

	bool Maxmized : 1;
	bool Minimized : 1;
	bool FullScreen : 1;
	bool Borderless : 1;
} WindowDesc;

typedef struct MonitorDesc
{
	Rect MonitorRect;
} MonitorDesc;
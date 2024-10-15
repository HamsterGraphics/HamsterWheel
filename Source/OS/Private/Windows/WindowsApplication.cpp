/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include <IApplication.h>
#include <IOperatingSystem.h>
#include <IWindow.h>

#include <Math/Math.h>

#include "WindowsUtils.h"

#include <cstring>
#include <memory>

///////////////////////////////////////////////////////
// CPU
///////////////////////////////////////////////////////
static CPUInfo g_cpuInfo;
CPUInfo* CPU_GetInfo()
{
	return &g_cpuInfo;
}

///////////////////////////////////////////////////////
// DRAM
///////////////////////////////////////////////////////
static DRAMInfo g_dramInfo;
DRAMInfo* DRAM_GetInfo()
{
	return &g_dramInfo;
}

///////////////////////////////////////////////////////
// Time
///////////////////////////////////////////////////////
static TimeInfo g_timeInfo;
TimeInfo* Time_GetInfo()
{
	return &g_timeInfo;
}

///////////////////////////////////////////////////////
// Window
///////////////////////////////////////////////////////
void Window_AdjustRect(const hg::AppSettings& appSettings, WindowDesc& windowDesc)
{
	windowDesc.WindowRect.X = appSettings.WindowPosX;
	windowDesc.WindowRect.Y = appSettings.WindowPosY;
	windowDesc.WindowRect.Width = appSettings.WindowWidth;
	windowDesc.WindowRect.Height = appSettings.WindowHeight;

	if (windowDesc.WindowRect.Width <= 0 ||
		windowDesc.WindowRect.Height <= 0)
	{
		windowDesc.WindowRect.Width = 960;
		windowDesc.WindowRect.Height = 540;
	}
}

///////////////////////////////////////////////////////
// Entry Point
///////////////////////////////////////////////////////
int AppMain(int argc, char** argv, hg::IApplication* pApp)
{
	UNUSED(argc);
	UNUSED(argv);

	CPU_InitInfo(&g_cpuInfo);
	DRAM_InitInfo(&g_dramInfo);
	Time_Init(&g_timeInfo);

	Window_InitSystem();

	pApp->Init();

	auto& appSettings = pApp->GetSettings();
	WindowDesc windowDesc;
	memset(&windowDesc, 0, sizeof(windowDesc));
	windowDesc.Name = pApp->GetName();
	Window_AdjustRect(appSettings, windowDesc);

	if (!appSettings.Faceless)
	{
		Window_Create(&windowDesc);
	}

	int64 lastCounter = Time_QueryCounter();
	bool quit = false;
	while (!quit)
	{
		int64 currentCounter = Time_QueryCounter();
		int64 deltaCounter = currentCounter - lastCounter;
		float deltaSeconds = static_cast<float>(deltaCounter) * g_timeInfo.SecondsPerCycle;
		lastCounter = currentCounter;

		quit = Window_HandleMessages();
		pApp->Update(deltaSeconds);
		pApp->Render();
	}

	pApp->Shutdown();

	return 0;
}
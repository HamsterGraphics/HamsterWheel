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
// Power
///////////////////////////////////////////////////////
static PowerInfo g_powerInfo;
PowerInfo* Power_GetInfo()
{
	return &g_powerInfo;
}

///////////////////////////////////////////////////////
// Monitor
///////////////////////////////////////////////////////
static MonitorInfo g_monitorInfo[MAX_MONITOR_COUNT];
static uint32 g_monitorCount;
MonitorInfo* Monitor_GetInfo(uint32 monitorIndex)
{
	return &g_monitorInfo[monitorIndex];
}

uint32 Monitor_GetCount()
{
	return g_monitorCount;
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
// Console
///////////////////////////////////////////////////////
static ConsoleInfo g_consoleInfo;

///////////////////////////////////////////////////////
// Window
///////////////////////////////////////////////////////
void Window_AdjustRect(const hg::AppSettings& appSettings, WindowInfo& windowInfo)
{
	windowInfo.WindowRect.X = appSettings.WindowPosX;
	windowInfo.WindowRect.Y = appSettings.WindowPosY;
	windowInfo.WindowRect.Width = appSettings.WindowWidth;
	windowInfo.WindowRect.Height = appSettings.WindowHeight;

	if (windowInfo.WindowRect.Width <= 0 ||
		windowInfo.WindowRect.Height <= 0)
	{
		auto* pMonitorInfo = Monitor_GetInfo(0);
		int resolutionWidth = pMonitorInfo->WorkRect.Width;
		int resolutionHeight = pMonitorInfo->WorkRect.Height;

		constexpr float visibleRate = 0.5f;
		windowInfo.WindowRect.Width = static_cast<int>(resolutionWidth * visibleRate);
		windowInfo.WindowRect.Height = static_cast<int>(resolutionHeight * visibleRate);
		windowInfo.WindowRect.X = (resolutionWidth - windowInfo.WindowRect.Width) / 2;
		windowInfo.WindowRect.Y = (resolutionHeight - windowInfo.WindowRect.Height) / 2;
	}
}

///////////////////////////////////////////////////////
// Entry Point
///////////////////////////////////////////////////////
int AppMain(int argc, char** argv, hg::IApplication* pApp)
{
	UNUSED(argc);
	UNUSED(argv);

	auto& appSettings = pApp->GetSettings();
	pApp->Init();

	// Init subsystems
	Console_Init(&g_consoleInfo);
	if (!appSettings.EnableConsole)
	{
		Console_Shutdown(&g_consoleInfo);
	}
	else
	{
		Console_Show(&g_consoleInfo);
	}

	CPU_InitInfo(&g_cpuInfo);
	DRAM_InitInfo(&g_dramInfo);
	Power_UpdateStatus(&g_powerInfo);
	Monitor_InitInfo(g_monitorInfo, g_monitorCount);
	Time_Init(&g_timeInfo);
	Window_Init();
	
	WindowInfo windowInfo;
	memset(&windowInfo, 0, sizeof(windowInfo));
	windowInfo.Name = pApp->GetName();
	Window_AdjustRect(appSettings, windowInfo);

	if (!appSettings.Faceless)
	{
		Window_Create(&windowInfo);
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

	Console_Shutdown(&g_consoleInfo);

	return 0;
}
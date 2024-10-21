/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "IApplication.h"
#include "IOperatingSystem.h"
#include "IWindow.h"

#include "Math/Math.h"

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
// Log
///////////////////////////////////////////////////////
static LogInfo g_logInfo;
bool Log_Init(LogInfo* pInfo)
{
	return true;
}

///////////////////////////////////////////////////////
// Thread
///////////////////////////////////////////////////////
static uint32 g_mainThreadID;
bool Thread_Init()
{
	g_mainThreadID = Thread_GetCurrentID();
	return true;
}

bool Thread_IsInMainThread()
{
	return Thread_GetCurrentID() == g_mainThreadID;
}

///////////////////////////////////////////////////////
// Application
///////////////////////////////////////////////////////
static hg::IApplication* g_pApp;

///////////////////////////////////////////////////////
// Input
///////////////////////////////////////////////////////
static InputInfo g_inputInfo;
bool Input_Init(InputInfo* pInfo)
{
	memset(pInfo->InputValue, 0, sizeof(pInfo->InputValue));
	return true;
}

float Input_GetValue(InputKey key)
{
	return g_inputInfo.InputValue[key];
}

///////////////////////////////////////////////////////
// Entry Point
///////////////////////////////////////////////////////
int AppMain(int argc, char** argv, hg::IApplication* pApp)
{
	UNUSED(argc);
	UNUSED(argv);
	g_pApp = pApp;

	// Init application settings
	pApp->InitSettings();
	auto& appSettings = pApp->AppSettings;
	
	// Init subsystems
	Thread_Init();
	Console_Init(&g_consoleInfo);
	if (!appSettings.EnableConsole)
	{
		Console_Shutdown(&g_consoleInfo);
	}
	else
	{
		Console_Show(&g_consoleInfo);
	}
	Log_Init(&g_logInfo);

	CPU_InitInfo(&g_cpuInfo);
	DRAM_InitInfo(&g_dramInfo);
	Power_UpdateStatus(&g_powerInfo);
	Monitor_InitInfo(g_monitorInfo, g_monitorCount);
	Time_Init(&g_timeInfo);
	Input_Init(&g_inputInfo);
	
	// Init application
	pApp->Init();

	// Create application window
	WindowInfo windowInfo;
	memset(&windowInfo, 0, sizeof(windowInfo));
	windowInfo.Name = pApp->GetName();
	if (!appSettings.Faceless)
	{
		Window_Create(&windowInfo);
	}

	// Loop
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

	// Shutdown subsystems
	Console_Shutdown(&g_consoleInfo);

	// Shutdown application
	pApp->Shutdown();

	return 0;
}

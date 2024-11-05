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
// Thread
///////////////////////////////////////////////////////
static ThreadInfo g_threadInfo;
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
// Window
///////////////////////////////////////////////////////
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_ACTIVATE:
	{
		(LOWORD(wParam) != WA_INACTIVE);
		break;
	}
	case WM_SIZE:
	{
		uint32 newWidth = LOWORD(lParam);
		uint32 newHeight = HIWORD(lParam);
		break;
	}
	case WM_MOUSEMOVE:
	{
		g_inputInfo.InputValue[INPUT_MOUSE_X] = static_cast<float>(LOWORD(lParam));
		g_inputInfo.InputValue[INPUT_MOUSE_Y] = static_cast<float>(HIWORD(lParam));
		break;
	}
	case WM_MOUSEWHEEL:
	{
		g_inputInfo.InputValue[INPUT_MOUSE_WHEEL] = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		g_inputInfo.InputValue[INPUT_MOUSE_LB] = 1.f;
		break;
	}
	case WM_LBUTTONUP:
	{
		g_inputInfo.InputValue[INPUT_MOUSE_LB] = 0.f;
		break;
	}
	case WM_MBUTTONDOWN:
	{
		g_inputInfo.InputValue[INPUT_MOUSE_MB] = 1.f;
		break;
	}
	case WM_MBUTTONUP:
	{
		g_inputInfo.InputValue[INPUT_MOUSE_MB] = 0.f;
		break;
	}
	case WM_RBUTTONDOWN:
	{
		g_inputInfo.InputValue[INPUT_MOUSE_RB] = 1.f;
		break;
	}
	case WM_RBUTTONUP:
	{
		g_inputInfo.InputValue[INPUT_MOUSE_RB] = 0.f;
		break;
	}
	case WM_XBUTTONDOWN:
	{
		g_inputInfo.InputValue[INPUT_MOUSE_XB] = 1.f;
		break;
	}
	case WM_XBUTTONUP:
	{
		g_inputInfo.InputValue[INPUT_MOUSE_XB] = 0.f;
		break;
	}
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		break;
	}
	case WM_CHAR:
	{
		break;
	}
	case WM_DESTROY:
	case WM_CLOSE:
	{
		::PostQuitMessage(0);
		break;
	}
	}

	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

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
	g_pApp = pApp;

	Thread_Init();
	Console_Init(&g_consoleInfo);
	Log_Init(&g_consoleInfo);
	
	// Init application settings
	LOG_TRACE("Init application settings.");
	pApp->InitSettings();

	// Init subsystems
	LOG_TRACE("Collect CPU info.");
	constexpr float ByteToMB = 1024.f * 1024.f;
	CPU_InitInfo(&g_cpuInfo);
	LOG_INFO("[CPU] %s", g_cpuInfo.Brand);
	LOG_INFO("  Vendor : %s", g_cpuInfo.Vendor);
	LOG_INFO("  PhyscialProcessorCount : %u", g_cpuInfo.PhyscialProcessorCount);
	LOG_INFO("  LogicalProcessorCount : %u", g_cpuInfo.LogicalProcessorCount);
	LOG_INFO("  L1 Cache = %.3f MB", g_cpuInfo.L1CacheSize / ByteToMB);
	LOG_INFO("  L2 Cache = %.3f MB", g_cpuInfo.L2CacheSize / ByteToMB);
	LOG_INFO("  L3 Cache = %.3f MB", g_cpuInfo.L3CacheSize / ByteToMB);
	LOG_INFO("  ISA Features");
	LOG_INFO("    [F16C]\t= %u", g_cpuInfo.Features.F16C);
	LOG_INFO("    [FMA3]\t= %u", g_cpuInfo.Features.FMA3);
	LOG_INFO("    [LZCNT]\t= %u", g_cpuInfo.Features.LZCNT);
	LOG_INFO("    [POPCNT]\t= %u", g_cpuInfo.Features.POPCNT);
	LOG_INFO("    [AVX]\t= %u", g_cpuInfo.Features.AVX);
	LOG_INFO("    [AVX2]\t= %u", g_cpuInfo.Features.AVX2);
	LOG_INFO("    [AVX512CD]\t= %u", g_cpuInfo.Features.AVX512CD);
	LOG_INFO("    [AVX512ER]\t= %u", g_cpuInfo.Features.AVX512ER);
	LOG_INFO("    [AVX512F]\t= %u", g_cpuInfo.Features.AVX512F);
	LOG_INFO("    [AVX512PF]\t= %u", g_cpuInfo.Features.AVX512PF);
	LOG_INFO("    [SSE]\t= %u", g_cpuInfo.Features.SSE);
	LOG_INFO("    [SSE2]\t= %u", g_cpuInfo.Features.SSE2);
	LOG_INFO("    [SSE3]\t= %u", g_cpuInfo.Features.SSE3);
	LOG_INFO("    [SSSE3]\t= %u", g_cpuInfo.Features.SSSE3);
	LOG_INFO("    [SSE41]\t= %u", g_cpuInfo.Features.SSE41);
	LOG_INFO("    [SSE42]\t= %u", g_cpuInfo.Features.SSE42);

	LOG_TRACE("Collect DRAM info.");
	DRAM_InitInfo(&g_dramInfo);
	LOG_INFO("[DRAM]");
	LOG_INFO("  AvailableSize = %.3f MB", g_dramInfo.AvailableSize / ByteToMB);
	LOG_INFO("  TotalSize = %.3f MB", g_dramInfo.TotalSize / ByteToMB);

	LOG_TRACE("Collect Monitor info.");
	Monitor_InitInfo(g_monitorInfo, g_monitorCount);
	for (uint32 monitorIndex = 0; monitorIndex < g_monitorCount; ++monitorIndex)
	{
		const auto& monitorInfo = g_monitorInfo[monitorIndex];
		LOG_INFO("[Monitor] %u", monitorInfo.Index);
		LOG_INFO("  DisplayName = %s", monitorInfo.DisplayName);
		LOG_INFO("  AdapterName = %s", monitorInfo.AdapterName);
		LOG_INFO("  DPI = (%u, %u)", monitorInfo.DPI[0], monitorInfo.DPI[1]);
		LOG_INFO("  PhyscialSize = (w = %u, h = %u)", monitorInfo.PhyscialWidth, monitorInfo.PhyscialHeight);
		LOG_INFO("  MonitorRect = (x = %u, y = %u, w = %u, h = %u)", monitorInfo.MonitorRect.X, monitorInfo.MonitorRect.Y, monitorInfo.MonitorRect.Width, monitorInfo.MonitorRect.Height);
		LOG_INFO("  WorkRect = (x = %u, y = %u, w = %u, h = %u)", monitorInfo.WorkRect.X, monitorInfo.WorkRect.Y, monitorInfo.WorkRect.Width, monitorInfo.WorkRect.Height);
	}

	Power_UpdateStatus(&g_powerInfo);
	Time_Init(&g_timeInfo);

	auto& appSettings = pApp->AppSettings;
	Input_Init(&g_inputInfo);
	Window_Init(WindowProc);
	
	// Create application window
	WindowInfo windowInfo;
	memset(&windowInfo, 0, sizeof(windowInfo));
	windowInfo.Name = pApp->GetName();
	Window_AdjustRect(appSettings, windowInfo);
	if (!appSettings.Faceless)
	{
		LOG_TRACE("Create application window.");
		LOG_INFO("[Window] %s", windowInfo.Name);
		LOG_INFO("  FullScreen = %u", windowInfo.FullScreen);
		LOG_INFO("  Borderless = %u", windowInfo.Borderless);
		LOG_INFO("  Maxmized = %u", windowInfo.Maxmized);
		LOG_INFO("  Minimized = %u", windowInfo.Minimized);
		LOG_INFO("  Hide = %u", windowInfo.Hide);
		LOG_INFO("  Rect = (x = %u, y = %u, w = %u, h = %u)", windowInfo.WindowRect.X, windowInfo.WindowRect.Y, windowInfo.WindowRect.Width, windowInfo.WindowRect.Height);
		Window_Create(&windowInfo);
	}

	// Init application
	pApp->Init(windowInfo.Handle);

	// Loop
	LOG_TRACE("Start application loop.");
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

	// Shutdown application
	pApp->Shutdown();
	LOG_TRACE("Shutdown application.");

	return 0;
}
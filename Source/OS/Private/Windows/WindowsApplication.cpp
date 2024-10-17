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

bool Window_Init()
{
	WNDCLASSW windowClass;
	memset(&windowClass, 0, sizeof(windowClass));
	windowClass.style = 0;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = (HINSTANCE)::GetModuleHandle(NULL);
	windowClass.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = HG_WINDOW_CLASS_NAME;
	::RegisterClassW(&windowClass);

	return true;
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

	// Init application
	auto& appSettings = pApp->GetSettings();
	pApp->Init();

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
	Window_Init();
	
	// Create application window
	WindowInfo windowInfo;
	memset(&windowInfo, 0, sizeof(windowInfo));
	windowInfo.Name = pApp->GetName();
	Window_AdjustRect(appSettings, windowInfo);
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
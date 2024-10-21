/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "IOperatingSystem.h"

#include <windows.h>

#include <cstring>
#include <memory>

///////////////////////////////////////////////////////////////////////////////////
// Time
///////////////////////////////////////////////////////////////////////////////////
bool Time_Init(TimeInfo* pInfo)
{
	LARGE_INTEGER frequency;
	bool result = ::QueryPerformanceFrequency(&frequency);
	pInfo->Frequency = frequency.QuadPart;
	pInfo->SecondsPerCycle = 1.0f / static_cast<float>(pInfo->Frequency);
	return result;
}

int64 Time_QueryCounter()
{
	LARGE_INTEGER counter;
	::QueryPerformanceCounter(&counter);
	return counter.QuadPart;
}

///////////////////////////////////////////////////////////////////////////////////
// Console
///////////////////////////////////////////////////////////////////////////////////
bool Console_Init(ConsoleInfo* pInfo)
{
	pInfo->OutputHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
	pInfo->WindowHandle = ::GetConsoleWindow();
	return true;
}

void Console_Shutdown(ConsoleInfo* pInfo)
{
	::CloseHandle((HANDLE)pInfo->OutputHandle);
}

void Console_Show(ConsoleInfo* pInfo)
{
	::ShowWindow((HWND)pInfo->WindowHandle, SW_SHOW);
}

void Console_Hide(ConsoleInfo* pInfo)
{
	::ShowWindow((HWND)pInfo->WindowHandle, SW_HIDE);
}

///////////////////////////////////////////////////////////////////////////////////
// Environment Variable
///////////////////////////////////////////////////////////////////////////////////
bool EnvironmentVariable_Contains(const char* key)
{
	return ::GetEnvironmentVariableA(key, NULL, 0) > 0;
}

const char* EnvironmentVariable_Get(const char* key)
{
	// Limit according to http://msdn.microsoft.com/en-us/library/ms683188.aspx
	const DWORD buffSize = 65535;
	static char buffer[buffSize];
	return ::GetEnvironmentVariableA(key, buffer, buffSize) ? buffer : nullptr;
}

bool EnvironmentVariable_Set(const char* key, const char* value)
{
	return ::SetEnvironmentVariableA(key, value);
}

///////////////////////////////////////////////////////////////////////////////////
// Module
///////////////////////////////////////////////////////////////////////////////////
void* Module_Load(const char* pFilePath)
{
	return ::LoadLibraryA(pFilePath);
}

void Module_Unload(void* pModule)
{
	::FreeLibrary((HMODULE)pModule);
}

void* Module_GetFunctionAddress(void* pModule, const char* pFunctionName)
{
	return ::GetProcAddress((HMODULE)pModule, pFunctionName);
}
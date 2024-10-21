/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "IOperatingSystem.h"

#include <cstring>
#include <memory>

///////////////////////////////////////////////////////////////////////////////////
// Time
///////////////////////////////////////////////////////////////////////////////////
bool Time_Init(TimeInfo* pInfo)
{
	return true;
}

int64 Time_QueryCounter()
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////
// Console
///////////////////////////////////////////////////////////////////////////////////
bool Console_Init(ConsoleInfo* pInfo)
{
	return true;
}

void Console_Shutdown(ConsoleInfo* pInfo)
{
}

void Console_Show(ConsoleInfo* pInfo)
{
}

void Console_Hide(ConsoleInfo* pInfo)
{
}

///////////////////////////////////////////////////////////////////////////////////
// Environment Variable
///////////////////////////////////////////////////////////////////////////////////
bool EnvironmentVariable_Contains(const char* key)
{
    return true;
}

const char* EnvironmentVariable_Get(const char* key)
{
	return nullptr;
}

bool EnvironmentVariable_Set(const char* key, const char* value)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////
// Module
///////////////////////////////////////////////////////////////////////////////////
void* Module_Load(const char* pFilePath)
{
	return nullptr;
}

void Module_Unload(void* pModule)
{
}

void* Module_GetFunctionAddress(void* pModule, const char* pFunctionName)
{
	return nullptr;
}

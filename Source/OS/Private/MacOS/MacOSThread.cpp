/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "IOperatingSystem.h"

///////////////////////////////////////////////////////////////////////////////////
// Thread
///////////////////////////////////////////////////////////////////////////////////
uint32 Thread_GetCurrentID()
{
	return 0;
}

const char* Thread_GetCurrentName()
{
	return nullptr;
}

void Thread_SetCurrentName(const char* pName)
{
}

void Thread_Sleep(uint32 seconds)
{
}

bool Thread_Create(ThreadInfo* pInfo)
{
	return true;
}

void Thread_Join(ThreadInfo* pInfo)
{
}

void Thread_Detach(ThreadInfo* pInfo)
{
}
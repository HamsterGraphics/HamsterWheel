/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "IOperatingSystem.h"

#include <memory>
#include <process.h>

///////////////////////////////////////////////////////////////////////////////////
// Thread
///////////////////////////////////////////////////////////////////////////////////
uint32 Thread_GetCurrentID()
{
	return ::GetCurrentThreadId();
}

char* Thread_GetCurrentNameForWrite()
{
	__declspec(thread) static char threadLocalName[MAX_THREAD_NAME_LENGTH];
	return threadLocalName;
}

const char* Thread_GetCurrentName()
{
	return Thread_GetCurrentNameForWrite();
}

void Thread_SetCurrentName(const char* pName)
{
	auto* pTarget = Thread_GetCurrentNameForWrite();
	strcpy_s(pTarget, MAX_THREAD_NAME_LENGTH, pName);
}

void Thread_Sleep(uint32 seconds)
{
	::Sleep(seconds);
}

uint32 WINAPI ThreadFunctionStatic(void* pData)
{
	ThreadInfo info = *(reinterpret_cast<ThreadInfo*>(pData));
	::free(pData);

	Thread_SetCurrentName(info.ThreadName);
	info.ThreadMain(info.Data);
	return 0;
}

bool Thread_Create(ThreadInfo* pInfo)
{
	auto* pThreadInfoHeapCopy = (ThreadInfo*)::malloc(sizeof(ThreadInfo));
	*pThreadInfoHeapCopy = *pInfo;

	pInfo->ThreadHandle = reinterpret_cast<void*>(_beginthreadex(0, 0, ThreadFunctionStatic, pThreadInfoHeapCopy, 0, 0));
	return pInfo->ThreadHandle != nullptr;
}

void Thread_Join(ThreadInfo* pInfo)
{
	::WaitForSingleObject((HANDLE)pInfo->ThreadHandle, INFINITE);
	::CloseHandle((HANDLE)pInfo->ThreadHandle);
}

void Thread_Detach(ThreadInfo* pInfo)
{
	::CloseHandle((HANDLE)pInfo->ThreadHandle);
}

///////////////////////////////////////////////////////////////////////////////////
// Mutex
///////////////////////////////////////////////////////////////////////////////////
bool Mutex_Create(Mutex* pMutex)
{
	constexpr uint64 DefaultSpinCount = 32;
	return ::InitializeCriticalSectionAndSpinCount((CRITICAL_SECTION*)&pMutex->Handle, (DWORD)DefaultSpinCount);
}

void Mutex_Destroy(Mutex* pMutex)
{
	::DeleteCriticalSection((CRITICAL_SECTION*)&pMutex->Handle);
	memset(&pMutex->Handle, 0, sizeof(pMutex->Handle));
}

void Mutex_Acquire(Mutex* pMutex)
{
	::EnterCriticalSection((CRITICAL_SECTION*)&pMutex->Handle);
}

void Mutex_Release(Mutex* pMutex)
{
	::LeaveCriticalSection((CRITICAL_SECTION*)&pMutex->Handle);
}
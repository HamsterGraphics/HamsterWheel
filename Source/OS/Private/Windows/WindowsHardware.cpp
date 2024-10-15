/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include <IOperatingSystem.h>

#include <Containers/Bitset.h>

#include "WindowsUtils.h"

#include <intrin.h>

#include <cstring>
#include <memory>

 ///////////////////////////////////////////////////////////////////////////////////
 // Forward Declaration
 ///////////////////////////////////////////////////////////////////////////////////
uint32 CountSetBits(CPUInfo* pInfo, ULONG_PTR bitMask);

///////////////////////////////////////////////////////////////////////////////////
// CPU
///////////////////////////////////////////////////////////////////////////////////
bool CPU_InitInfo(CPUInfo* pInfo)
{
	int CPU_IDData[4];
	int CPU_IndexData[4];

	// Calling __cpuid with 0x0 as the function_id argument
	// gets the number of the highest valid function ID.
	__cpuid(CPU_IDData, 0);

	__cpuidex(CPU_IndexData, 0, 0);

	memset(pInfo->Vendor, 0, sizeof(pInfo->Vendor));
	*reinterpret_cast<int*>(pInfo->Vendor) = CPU_IndexData[1];
	*reinterpret_cast<int*>(pInfo->Vendor + 4) = CPU_IndexData[3];
	*reinterpret_cast<int*>(pInfo->Vendor + 8) = CPU_IndexData[2];
	pInfo->IsAMD = 0 == strcmp(pInfo->Vendor, "AuthenticAMD");
	pInfo->IsIntel = 0 == strcmp(pInfo->Vendor, "GenuineIntel");

	hg::Bitset<32> CPU_f_1_ECX;
	hg::Bitset<32> CPU_f_1_EDX;
	hg::Bitset<32> CPU_f_7_EBX;
	hg::Bitset<32> CPU_f_7_ECX;
	hg::Bitset<32> CPU_f_81_ECX;
	hg::Bitset<32> CPU_f_81_EDX;

	// load bitset with flags for function 0x00000001
	if (CPU_IDData[0] >= 0x00000001)
	{
		__cpuidex(CPU_IndexData, 0x00000001, 0);
		CPU_f_1_ECX = CPU_IndexData[2];
		CPU_f_1_EDX = CPU_IndexData[3];
	}

	// load bitset with flags for function 0x00000007
	if (CPU_IDData[0] >= 0x00000007)
	{
		__cpuidex(CPU_IndexData, 0x00000007, 0);
		CPU_f_7_EBX = CPU_IndexData[1];
		CPU_f_7_ECX = CPU_IndexData[2];
	}

	// Calling __cpuid with 0x80000000 as the function_id argument
	// gets the number of the highest valid extended ID.
	__cpuid(CPU_IDData, 0x80000000);
	memset(pInfo->Brand, 0, sizeof(pInfo->Brand));

	// load bitset with flags for function 0x80000001
	if (CPU_IDData[0] >= 0x80000001)
	{
		__cpuidex(CPU_IndexData, 0x80000001, 0);
		CPU_f_81_ECX = CPU_IndexData[2];
		CPU_f_81_EDX = CPU_IndexData[3];
	}

	// Interpret CPU brand string if reported
	if (CPU_IDData[0] >= 0x80000004)
	{
		__cpuidex(CPU_IndexData, 0x80000002, 0);
		memcpy(pInfo->Brand, CPU_IndexData, sizeof(CPU_IndexData));

		__cpuidex(CPU_IndexData, 0x80000003, 0);
		memcpy(pInfo->Brand + 16, CPU_IndexData, sizeof(CPU_IndexData));

		__cpuidex(CPU_IndexData, 0x80000004, 0);
		memcpy(pInfo->Brand + 32, CPU_IndexData, sizeof(CPU_IndexData));
	}

	pInfo->HasAVX = CPU_f_1_ECX[28];
	pInfo->HasAVX2 = CPU_f_7_EBX[5];
	pInfo->HasAVX512CD = CPU_f_7_EBX[28];
	pInfo->HasAVX512ER = CPU_f_7_EBX[27];
	pInfo->HasAVX512F = CPU_f_7_EBX[16];
	pInfo->HasAVX512PF = CPU_f_7_EBX[26];
	pInfo->HasSSE = CPU_f_1_EDX[25];
	pInfo->HasSSE2 = CPU_f_1_EDX[26];
	pInfo->HasSSE3 = CPU_f_1_ECX[0];
	pInfo->HasSSE41 = CPU_f_1_ECX[19];
	pInfo->HasSSE42 = CPU_f_1_ECX[20];
	pInfo->HasSSSE3 = CPU_f_1_ECX[9];
	pInfo->HasFMA = CPU_f_1_ECX[12];
	pInfo->HasF16C = CPU_f_1_ECX[29];
	pInfo->HasPOPCNT = CPU_f_1_ECX[23];
	pInfo->HasLZCNT = pInfo->IsIntel && CPU_f_81_ECX[5];

	// Query processor information
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION processorInfoBuffer = NULL;
	DWORD returnLength = 0;
	::GetLogicalProcessorInformation(processorInfoBuffer, &returnLength);
	if (returnLength > 0)
	{
		processorInfoBuffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(returnLength);
		::GetLogicalProcessorInformation(processorInfoBuffer, &returnLength);
	}

	pInfo->PhyscialProcessorCount = 0;
	pInfo->LogicalProcessorCount = 0;
	pInfo->L1CacheSize = 0;
	pInfo->L2CacheSize = 0;
	pInfo->L3CacheSize = 0;
	if (processorInfoBuffer != NULL)
	{
		DWORD processorPackageCount = 0;
		PCACHE_DESCRIPTOR Cache;

		DWORD byteOffset = 0;
		PSYSTEM_LOGICAL_PROCESSOR_INFORMATION processorInfoPointer = processorInfoBuffer;
		while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength)
		{
			switch (processorInfoPointer->Relationship)
			{
			case RelationProcessorCore:
				pInfo->PhyscialProcessorCount++;

				// A hyperthreaded core supplies more than one logical processor.
				pInfo->LogicalProcessorCount += CountSetBits(pInfo, processorInfoPointer->ProcessorMask);
				break;

			case RelationCache:
				// Cache data is in ptr->Cache, one CACHE_DESCRIPTOR structure for each cache. 
				Cache = &processorInfoPointer->Cache;
				if (Cache->Level == 1)
				{
					pInfo->L1CacheSize += Cache->Size;
				}
				else if (Cache->Level == 2)
				{
					pInfo->L2CacheSize += Cache->Size;
				}
				else if (Cache->Level == 3)
				{
					pInfo->L3CacheSize += Cache->Size;
				}

				break;

			case RelationProcessorPackage:
				// Logical processors share a physical package.
				processorPackageCount++;
				break;

			default:
				break;
			}
			byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
			++processorInfoPointer;
		}

		free(processorInfoBuffer);
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////
// DRAM
///////////////////////////////////////////////////////////////////////////////////
bool DRAM_InitInfo(DRAMInfo* pInfo)
{
	MEMORYSTATUSEX stat;
	stat.dwLength = sizeof(stat);
	::GlobalMemoryStatusEx(&stat);

	pInfo->TotalSize = stat.ullTotalPhys;
	pInfo->AvailableSize = stat.ullAvailPhys;

	return true;
}

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
// Utils
///////////////////////////////////////////////////////////////////////////////////
uint32 CountSetBits(CPUInfo* pInfo, ULONG_PTR bitMask)
{
	if (pInfo->HasPOPCNT)
	{
		return static_cast<uint32>(__popcnt64(bitMask));
	}

	DWORD LSHIFT = sizeof(ULONG_PTR) * 8 - 1;
	uint32 bitSetCount = 0;
	ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;
	DWORD i;

	for (i = 0; i <= LSHIFT; ++i)
	{
		bitSetCount += ((bitMask & bitTest) ? 1 : 0);
		bitTest /= 2;
	}

	return bitSetCount;
}
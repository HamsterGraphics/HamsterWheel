/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "IOperatingSystem.h"

///////////////////////////////////////////////////////////////////////////////////
// CPU
///////////////////////////////////////////////////////////////////////////////////
bool CPU_InitInfo(CPUInfo* pInfo)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////
// DRAM
///////////////////////////////////////////////////////////////////////////////////
bool DRAM_InitInfo(DRAMInfo* pInfo)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////
// Power
///////////////////////////////////////////////////////////////////////////////////
bool Power_UpdateStatus(PowerInfo* pInfo)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////
// Monitor
///////////////////////////////////////////////////////////////////////////////////
bool Monitor_InitInfo(MonitorInfo* pInfo, uint32& monitorCount)
{
	return true;
}
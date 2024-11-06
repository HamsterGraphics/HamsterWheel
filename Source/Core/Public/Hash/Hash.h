/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "../Base/APIDefines.h"
#include "../Base/BasicTypes.h"

C_ABI HG_CORE_API uint32 HG_CDECL Hash_FastCRC32(const char* data, uint64 length);
C_ABI HG_CORE_API uint32 HG_CDECL Hash_FNV1A32(const char* data, uint64 length, uint32 seed);
C_ABI HG_CORE_API uint64 HG_CDECL Hash_FNV1A64(const char* data, uint64 length, uint64 seed);
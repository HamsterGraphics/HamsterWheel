/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "FastCRC32.hpp"
#include "FNV1A.hpp"
#include "Hash/Hash.h"

uint32 Hash_FastCRC32(const char* data, uint64 length)
{
    return hg::FastCRCHash<uint32>(data, length);
}

uint32 Hash_FNV1A32(const char* data, uint64 length, uint32 seed)
{
    return hg::FNV1AHash<uint32>(data, length, seed);
}

uint64 Hash_FNV1A64(const char* data, uint64 length, uint64 seed)
{
    return hg::FNV1AHash<uint64>(data, length, seed);
}
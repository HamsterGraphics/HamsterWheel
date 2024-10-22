/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "Base/BasicTypes.h"

#include <bitset>

namespace hg
{

template<uint32 Bits>
using Bitset = std::bitset<Bits>;

}
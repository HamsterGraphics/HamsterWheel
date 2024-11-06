/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "Allocator.h"

#include <vector>

namespace hg
{

template<typename T>
using Vector = std::vector<T, STLAllocator<T>>;

}
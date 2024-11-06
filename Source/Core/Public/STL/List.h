/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "Allocator.h"

#include <list>

namespace hg
{

template<typename T>
using List = std::list<T, STLAllocator<T>>;

}
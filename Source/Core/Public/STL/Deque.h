/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "Allocator.h"

#include <deque>

namespace hg
{

template<typename T>
using Deque = std::deque<T, STLAllocator<T>>;

}
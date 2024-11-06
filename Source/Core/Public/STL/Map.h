/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "Allocator.h"

#include <map>

namespace hg
{

template<typename K, typename V>
using Map = std::map<K, V, STLAllocator<T>>;

}
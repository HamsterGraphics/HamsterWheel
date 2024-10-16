/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include <unordered_map>

namespace hg
{

template<typename K, typename V>
using HashMap = std::unordered_map<K, V>;

}
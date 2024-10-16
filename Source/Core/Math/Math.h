/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include <Base/BasicTypes.h>

namespace hg
{

// Equals to "value * numer / denom". Prevent int64 overflow.
// https://gist.github.com/jspohr/3dc4f00033d79ec5bdaf67bc46c813e3
int64 Int64MulDiv(int64 value, int64 numer, int64 denom)
{
	int64 q = value / denom;
	int64 r = value % denom;
	return q * numer + r * numer / denom;
}

}
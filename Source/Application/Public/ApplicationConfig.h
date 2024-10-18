/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include <Base/APIDefines.h>
#include <Base/BasicTypes.h>

#ifdef HG_APP_EXPORT
#define HG_APP_API API_EXPORT
#else
#define HG_APP_API API_IMPORT
#endif
/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "Base/APIDefines.h"
#include "Base/BasicTypes.h"

#if defined(HG_SCENE_EXPORT)
#define HG_SCENE_API API_EXPORT
#else
#if defined(HG_MODULE_SHARED)
#define HG_SCENE_API API_IMPORT
#else
#define HG_SCENE_API
#endif
#endif

#define MAX_SCENE_OBJECT_NAME_LENGTH 64

#define MAX_UV_SET_COUNT 4
#define MAX_COLOR_SET_COUNT 8
#define MAX_VERTEX_INFLUENCE_COUNT 4
/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "../Base/BasicTypes.h"

#include <numeric>

typedef struct Rect
{
	int32 X;
	int32 Y;
	int32 Width;
	int32 Height;
} Rect;

typedef struct RGB
{
	float R;
	float G;
	float B;
} RGB;

typedef struct RGBA
{
	float R;
	float G;
	float B;
	float A;
} RGBA;

typedef struct Vec2
{
	float X;
	float Y;
} Vec2;

typedef struct Vec3
{
	float X;
	float Y;
	float Z;
} Vec3;

typedef struct Vec4
{
	float X;
	float Y;
	float Z;
	float W;
} Vec4;

typedef struct Matrix33
{
	float XX, XY, XZ;
	float YX, YY, YZ;
	float ZX, ZY, ZZ;
} Matrix33;

typedef struct Matrix44
{
	float XX, XY, XZ, XW;
	float YX, YY, YZ, YW;
	float ZX, ZY, ZZ, ZW;
	float WX, WY, WZ, WW;
} Matrix44;

typedef struct Transform
{
	Vec3 Position;
	Vec3 Rotation;
	Vec3 Scale;
} Transform;

typedef struct AABB
{
	Vec2 Min;
	Vec2 Max;
} AABB;

inline constexpr uint32 RoundUp(uint32 value, uint32 multiple)
{
	return ((value + multiple - 1) / multiple) * multiple;
}

inline constexpr uint32 RoundDown(uint32 value, uint32 multiple)
{
	return value - value % multiple;
}
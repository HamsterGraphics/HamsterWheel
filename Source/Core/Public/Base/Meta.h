/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "BasicTypes.h"

///////////////////////////////////////////////////////
// Parameter
///////////////////////////////////////////////////////
typedef enum ParameterType
{
	PARAMETER_TYPE_BOOL,
	PARAMETER_TYPE_INT,
	PARAMETER_TYPE_UINT,
	PARAMETER_TYPE_FLOAT,
	PARAMETER_TYPE_VEC3,
	PARAMETER_TYPE_VEC4
} ParameterType;

#define MAX_PARAMETER_NAME_LENGTH 64
#define MAX_PARAMETER_BUFFER_LENGTH 16

typedef struct Paramter
{
	ParameterType Type;
	char Name[MAX_PARAMETER_NAME_LENGTH];
	byte Buffer[MAX_PARAMETER_BUFFER_LENGTH];
} Parameter;
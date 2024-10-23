/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "GraphicsConfig.h"

///////////////////////////////////////////////////////
// Generic
///////////////////////////////////////////////////////
typedef enum GraphicsBackend
{
	GFX_BACKEND_D3D12,
	GFX_BACKEND_VULKAN,
	GFX_BACKEND_METAL
} GraphicsAPI;

typedef enum GPUMode
{
	GPU_MODE_SINGLE_ADAPTER,
	GPU_MODE_MULTI_ADAPTERS_LINKED,
	GPU_MODE_MULTI_ADAPTERS_UNLINKED
} GPUMode;

typedef enum IndexType
{
	INDEX_TYPE_UINT16,
	INDEX_TYPE_UINT32
} IndexType;

typedef enum CompareMode
{
	COMPARE_MODE_NEVER,
	COMPARE_MODE_ALWAYS,
	COMPARE_MODE_EQUAL,
	COMPARE_MODE_NOT_EQUAL,
	COMPARE_MODE_LESS,
	COMPARE_MODE_LESS_EQUAL,
	COMPARE_MODE_GREATER,
	COMPARE_MODE_GREATER_EQUAL
} CompareMode;

///////////////////////////////////////////////////////
// Color
///////////////////////////////////////////////////////
typedef enum ColorSpace
{
	COLOR_SPACE_SDR_LINEAR,
	COLOR_SPACE_SDR_SRGB
} ColorSpace;

///////////////////////////////////////////////////////
// Command
///////////////////////////////////////////////////////
typedef enum CommandQueueType
{
	COMMAND_QUEUE_GRAPHICS,
	COMMAND_QUEUE_TRANSFER,
	COMMAND_QUEUE_COMPUTE,
} CommandQueueType;

typedef enum LoadActionType
{
	LOAD_ACTION_DONTCARE,
	LOAD_ACTION_LOAD,
	LOAD_ACTION_CLEAR
} LoadActionType;

typedef enum StoreActionType
{
	STORE_ACTION_DONTCARE,
	STORE_ACTION_STORE
} StoreActionType;

typedef enum QueryType
{
	QUERY_TYPE_TIMESTAMP,
	QUERY_TYPE_OCCLUSION
} QueryType;

///////////////////////////////////////////////////////
// Operation
///////////////////////////////////////////////////////
typedef enum BlendMode
{
	BLEND_MODE_ADD,
	BLEND_MODE_SUBTRACT
} BlendMode;

typedef enum StencilOperation
{
	STENCIL_OPERATION_KEEP,
	STENCIL_OPERATION_SET_ZERO,
	STENCIL_OPERATION_REPLACE,
	STENCIL_OPERATION_INVERT,
	STENCIL_OPERATION_INCRESE,
	STENCIL_OPERATION_DECRESE
} StencilOperation;

///////////////////////////////////////////////////////
// Primitive
///////////////////////////////////////////////////////
typedef enum PrimitiveTopology
{
	PRIMITIVE_TOPOLOGY_POINT_LIST,
	PRIMITIVE_TOPOLOGY_LINE_LIST,
	PRIMITIVE_TOPOLOGY_LINE_STRIP,
	PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
	PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
	PRIMITIVE_TOPOLOGY_PATCH_LIST
} PrimitiveTopology;

typedef enum FrontFace
{
	FRONT_FACE_CCW,
	FRONT_FACE_CW
} FrontFace;

typedef enum CullMode
{
	CULL_MODE_NONE,
	CULL_MODE_BACK,
	CULL_MODE_FRONT,
	CULL_MODE_BOTH
} CullMode;

typedef enum FillMode
{
	FILL_MODE_SOLID,
	FILL_MODE_WIREFRAME
} FillMode;

///////////////////////////////////////////////////////
// Texture
///////////////////////////////////////////////////////
typedef enum TextureDimension
{
	TEXTURE_DIMENSION_1D,
	TEXTURE_DIMENSION_2D,
	TEXTURE_DIMENSION_3D,
	TEXTURE_DIMENSION_CUBE,
	TEXTURE_DIMENSION_1D_ARRAY,
	TEXTURE_DIMENSION_2D_ARRAY,
	TEXTURE_DIMENSION_3D_ARRAY,
	TEXTURE_DIMENSION_CUBE_ARRAY
} TextureDimension;

typedef enum FilterType
{
	FILTER_NEAREST,
	FILTER_LINEAR
} FilterType;

typedef enum AddressMode
{
	ADDRESS_MODE_MIRROR,
	ADDRESS_MODE_REPEAT,
	ADDRESS_MODE_CLAMP_TO_EDGE,
	ADDRESS_MODE_CLAMP_TO_BORDER
} AddressMode;

typedef enum MipmapMode
{
	MIPMAP_MODE_NEAREST,
	MIPMAP_MODE_LINEAR
} MipmapMode;

///////////////////////////////////////////////////////
// Shader
///////////////////////////////////////////////////////
typedef enum ShaderStage
{
	SHADER_STAGE_VERTEX,
	SHADER_STAGE_FRAGMENT,
	SHADER_STAGE_COMPUTE
} ShaderStage;

typedef enum ShaderSemantic
{
	SHADER_SEMANTIC_POSITION,
	SHADER_SEMANTIC_NORMAL,
	SHADER_SEMANTIC_TANGENT,
	SHADER_SEMANTIC_BITANGENT,
	SHADER_SEMANTIC_JOINTS,
	SHADER_SEMANTIC_WEIGHTS,
	SHADER_SEMANTIC_COLOR,
	SHADER_SEMANTIC_TEXCOORD0,
	SHADER_SEMANTIC_TEXCOORD1,
	SHADER_SEMANTIC_TEXCOORD2,
	SHADER_SEMANTIC_TEXCOORD3
} ShaderSemantic;

typedef enum ShaderModel
{
	SHADER_MODEL_5_1, // Compatiable
	SHADER_MODEL_6_0,
	SHADER_MODEL_6_1,
	SHADER_MODEL_6_2,
	SHADER_MODEL_6_3, // DXR
	SHADER_MODEL_6_4, // VRS
	SHADER_MODEL_6_5,
	SHADER_MODEL_6_6
} ShaderModel;

///////////////////////////////////////////////////////
// GPU
///////////////////////////////////////////////////////
typedef struct GPUInfo
{
	uint64 VRAMSize;
	uint32 WaveLaneCount;
} GPUInfo;

///////////////////////////////////////////////////////
// Context
///////////////////////////////////////////////////////
C_ABI HG_GFX_API bool HG_CALLDECL Graphics_Init(const GraphicsContextCreateInfo& createInfo, GraphicsContext* pContext);
C_ABI HG_GFX_API void HG_CALLDECL Graphics_Shutdown(GraphicsContext* pContext);

///////////////////////////////////////////////////////
// Instance
///////////////////////////////////////////////////////
typedef struct InstanceInfo
{
} InstanceInfo;

C_ABI HG_GFX_API bool HG_CALLDECL Graphics_CreateInstance(InstanceInfo* pInfo);

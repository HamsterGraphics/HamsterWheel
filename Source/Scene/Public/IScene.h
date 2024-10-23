﻿/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "SceneConfig.h"
#include "Base/Meta.h"
#include "Math/Math.h"

///////////////////////////////////////////////////////////////////////////////////
// Node
///////////////////////////////////////////////////////////////////////////////////
typedef enum NodeType
{
	NODE_TYPE_TRANSFORM,
	NODE_TYPE_CAMERA,
	NODE_TYPE_LIGHT,
	NODE_TYPE_MESH,
	NODE_TYPE_JOINT,
} NodeType;

typedef struct Node
{
	NodeType Type;
	uint32 ChildCount;
	Node* Parent;
	Node* Children;
	void* Data;
	char Name[MAX_SCENE_OBJECT_NAME_LENGTH];
	Transform Transform;
} Node;

///////////////////////////////////////////////////////////////////////////////////
// Camera
///////////////////////////////////////////////////////////////////////////////////
typedef struct Camera
{
	char Name[MAX_SCENE_OBJECT_NAME_LENGTH];
	float Aspect;
	float FOV;
	float NearPlane;
	float FarPlane;
} Camera;

///////////////////////////////////////////////////////////////////////////////////
// Light
///////////////////////////////////////////////////////////////////////////////////
typedef enum LightType
{
	LIGHT_TYPE_DIRECTIONAL,
	LIGHT_TYPE_POINT,
	LIGHT_TYPE_SPOT,
};

typedef struct Light
{
	char Name[MAX_SCENE_OBJECT_NAME_LENGTH];
	LightType Type;
	float Intensity;
	RGB Color;
} Light;

///////////////////////////////////////////////////////////////////////////////////
// Texture
///////////////////////////////////////////////////////////////////////////////////
typedef enum TextureType
{
	TEXTURE_TYPE_FILE,
};

typedef struct Texture
{
	char Name[MAX_SCENE_OBJECT_NAME_LENGTH];
	TextureType Type;
} Texture;

///////////////////////////////////////////////////////////////////////////////////
// Material
///////////////////////////////////////////////////////////////////////////////////
typedef struct Material
{
	char Name[MAX_SCENE_OBJECT_NAME_LENGTH];
	uint32 ParameterCount;
	uint32 TextureCount;
	Paramter* Parameters;
	Texture* Textures;
} Material;

///////////////////////////////////////////////////////////////////////////////////
// Mesh
///////////////////////////////////////////////////////////////////////////////////
typedef struct SubMesh
{
	uint32* Indices;
	uint32 TriangleCount;
	Material* Material;
} SubMesh;

typedef struct Mesh
{
	char Name[MAX_SCENE_OBJECT_NAME_LENGTH];
	AABB Bounds;
	uint32 VertexCount;
	uint32 UVSetCount;
	uint32 ColorSetCount;
	uint32 SubMeshCount;
	Vec3* Positions;
	Vec3* Normals;
	Vec3* Tangents;
	Vec3* BiTangents;
	Vec2* UVs[MAX_UV_SET_COUNT];
	RGB* Colors[MAX_COLOR_SET_COUNT];
	SubMesh* SubMeshes;
	Skin* Skin;
};

///////////////////////////////////////////////////////////////////////////////////
// Joint
///////////////////////////////////////////////////////////////////////////////////
typedef struct Joint
{
	float LimbSize;
	char Name[MAX_SCENE_OBJECT_NAME_LENGTH];
	Matrix44 Offset;
} Joint;

///////////////////////////////////////////////////////////////////////////////////
// Skeleton
///////////////////////////////////////////////////////////////////////////////////
typedef struct Skeleton
{
	uint32 JointCount;
	Joint* Joints;
} Skeleton;

///////////////////////////////////////////////////////////////////////////////////
// Skin
///////////////////////////////////////////////////////////////////////////////////
typedef struct Skin
{
	Skeleton* BindSkeleton;
	uint32* Influences[MAX_VERTEX_INFLUENCE_COUNT];
	float* Weights[MAX_VERTEX_INFLUENCE_COUNT];
} Skin;

///////////////////////////////////////////////////////////////////////////////////
// Track
///////////////////////////////////////////////////////////////////////////////////
typedef struct Track
{
	uint32 TranslationKeyCount;
	Vec3* TranslationKeys;

	uint32 RotationKeyCount;
	Vec3* RotationKeys;

	uint32 ScaleKeyCount;
	Vec3* ScaleKeys;

	char Name[MAX_SCENE_OBJECT_NAME_LENGTH];
} Track;
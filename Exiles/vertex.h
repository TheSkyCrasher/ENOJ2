#pragma once

#include "math.h"
#include <cassert>

struct Vertex
{
public:
	Vertex() {}

	Vertex(float x, float y, float z, float u, float v, float nx = 0.0f, float ny = 0.0f, float nz = 0.0f)
	{
		this->pos = Vector3f(x, y, z);
		this->texCoord = Vector2f(u, v);
		this->normal = Vector3f(nx, ny, nz);
	}

	Vector3f pos;
	Vector2f texCoord;
	Vector3f normal;
	Vector3f tangent;
	Vector4i boneIds;
	Vector4f weights;
};
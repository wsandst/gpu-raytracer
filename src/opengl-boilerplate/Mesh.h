#pragma once
#include "VBOWrapper.h"

struct Mesh
{
public:
	Vec3i pos;

	std::vector<GeometryVertexAttrib> vertices;

	Mesh(Vec3i pos, std::vector<GeometryVertexAttrib>& vertices)
	{
		this->vertices = vertices;

		this->pos = pos;
	}

	Mesh(Vec3i pos, std::vector<float>& fVertices)
	{
		vertices = std::vector<GeometryVertexAttrib>();
		for (size_t i = 0; i < fVertices.size(); i += 3)
		{
			vertices.push_back(GeometryVertexAttrib(fVertices[i], fVertices[i + 1], fVertices[i + 2]));
		}
		this->pos = pos;
	}
	Mesh() {};
	~Mesh() {};
};


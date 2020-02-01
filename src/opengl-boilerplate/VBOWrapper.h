#pragma once
#include "Helpers.h"

#define GLEW_STATIC 
#include "GL/glew.h"

#include "SDL.h"
#include "SDL_opengl.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

struct GeometryVertexAttrib {
	GeometryVertexAttrib(float x, float y, float z) {
		position[0] = x;
		position[1] = y;
		position[2] = z;
	}

	float position[3];
};


class GeometryVBO
{
public:
	unsigned int VBO;
	unsigned int VAO;
	glm::mat4 translation;
	int size;

	GeometryVBO() {};
	GeometryVBO(Vec3i pos, std::vector<GeometryVertexAttrib>& vertices)
	{
		create(vertices);
		setPosition(pos);
	};
	~GeometryVBO()
	{
	};
	void create(std::vector<GeometryVertexAttrib>& vertices)
	{
		size = vertices.size();
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		if (vertices.size() > 0)
		{
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GeometryVertexAttrib), &vertices[0], GL_STATIC_DRAW);
		}
		//Positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GeometryVertexAttrib), nullptr);
		glEnableVertexAttribArray(0);

	}
	void update(std::vector<GeometryVertexAttrib>& vertices)
	{
		size = vertices.size();
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		if (vertices.size() > 0)
		{
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STREAM_DRAW);
		}
	}
	void remove()
	{

	}
	void setPosition(Vec3i pos)
	{
		translation = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, pos.z));
	}
};
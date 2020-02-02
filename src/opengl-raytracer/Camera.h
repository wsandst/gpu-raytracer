#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

class Camera
{
private:
	glm::vec3 cameraPos = glm::vec3(3.0f, 2.0f, 7.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.5f, 0.0f);
	glm::mat4x4 viewMatrix, projectionMatrix;

public:
	glm::vec3 ray00, ray10, ray01, ray11;
	float FOV = 60.0f, yaw = -90.0f, pitch = 0.0f;
	int windowWidth, windowHeight;
	float sensitivity = 0.5f;
	float cameraStep = 1;

	void moveRight()
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraStep;
	}
	void moveLeft()
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraStep;
	}
	void moveUp()
	{
		cameraPos += cameraStep * cameraUp;
	}
	void moveDown()
	{
		cameraPos -= cameraStep * cameraUp;
	}
	void moveBack()
	{
		cameraPos -= cameraStep * cameraFront;
	}
	void moveForward()
	{
		cameraPos += cameraStep * cameraFront;
	}
	void updateView(float xOffset, float yOffset)
	{
		
		yaw += xOffset;
		pitch += yOffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);
	}

	glm::vec3& getCameraFront() {
		return cameraFront;
	}

	glm::mat4x4& getProjectionMatrix()
	{
		return projectionMatrix;
	}

	glm::mat4x4& getViewMatrix()
	{
		return viewMatrix;
	}

	void calculateCornerRay(float x, float y, glm::vec3 &res)
	{
		auto invViewProjMat = glm::transpose(glm::inverse(projectionMatrix * viewMatrix));
		glm::vec4 ray = glm::vec4(x, y, 0, 1) * invViewProjMat;
		res = ray * (1 / ray.w);
		res = res - cameraPos;
	}

	void getCornerRays()
	{
		calculateCornerRay(-1, -1, ray00);
		calculateCornerRay(1, -1, ray10);
		calculateCornerRay(-1, 1, ray01);
		calculateCornerRay(1, 1, ray11);
	}

	void calculateProjectionMatrix()
	{
		projectionMatrix = glm::perspective(glm::radians(FOV), (float)windowWidth / (float)windowHeight, 0.1f, 1000.0f);
	}

	void calculateViewMatrix()
	{
		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		int x = 4;
	}

	glm::vec3& getPosition()
	{
		return cameraPos;
	}

	Camera(int windowWidth, int windowHeight)
	{
		this->windowHeight = windowHeight;
		this->windowWidth = windowWidth;
	}

	Camera() {};
	~Camera() {};
};


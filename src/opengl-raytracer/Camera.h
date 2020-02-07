#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <algorithm>

class Camera
{
private:
	glm::vec3 cameraPos = glm::vec3(3.0f, 2.0f, 7.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.5f, 0.0f);
	glm::mat4x4 viewMatrix, projectionMatrix;

	float acceleration = 0.1;

public:
	glm::vec3 ray00, ray10, ray01, ray11;
	float FOV = 60.0f, yaw = -90.0f, pitch = 0.0f;
	int windowWidth, windowHeight;
	float sensitivity = 0.5f;
	float cameraStep = 1;
	float maxVelocity = 0.5;
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	bool keyForward = false, keyBackward = false, keyRight = false, keyLeft = false, keyUp = false, keyDown = false;
	
	void accelerate(float x, float y, float z)
	{
		if (std::abs(velocity.x + x) <= maxVelocity)
			velocity.x = velocity.x + x;
		if (std::abs(velocity.y + y) <= maxVelocity)
			velocity.y = velocity.y + y;
		if (std::abs(velocity.z + z) <= maxVelocity)
			velocity.z = velocity.z + z;
	}

	void decelerate(float x, float y, float z)
	{
		if (std::abs(velocity.x - x) <= std::abs(velocity.x))
			velocity.x = velocity.x - x;
		else
			velocity.x = 0;
		if (std::abs(velocity.y - y) <= std::abs(velocity.y))
			velocity.y = velocity.y - y;
		else
			velocity.y = 0;
		if (std::abs(velocity.z - z) <= std::abs(velocity.z))
			velocity.z = velocity.z - z;
		else
			velocity.z = 0;
	}

	void moveKey(bool key, float ax, float ay, float az, float vc)
	{
		if (key)
			accelerate(ax, ay, az);
		else if (vc > 0)
			decelerate(ax, ay, az);
	}

	void move()
	{
		moveKey(keyRight, acceleration, 0, 0, velocity.x);
		moveKey(keyLeft, -acceleration, 0, 0, -velocity.x);
		moveKey(keyUp, 0, acceleration, 0, velocity.y);
		moveKey(keyDown, 0, -acceleration, 0, -velocity.y);
		moveKey(keyForward, 0, 0, acceleration, velocity.z);
		moveKey(keyBackward, 0, 0, -acceleration, -velocity.z);

		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraStep * velocity.x;
		cameraPos += cameraUp * cameraStep * velocity.y;
		cameraPos += cameraFront * cameraStep * velocity.z;
	}

	void changeMaxVelocity(float factor)
	{
		maxVelocity *= factor;
		acceleration *= factor;
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
		updateView(0, 0);
	}

	Camera() 
	{
		updateView(0, 0);
	};
	~Camera() {};
};


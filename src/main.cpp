#include <iostream>
#include <vector>
#include <ctime>

#define NOMINMAX
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "stb_image_write.h"

#include "Renderer.h"
#include "InputHandler.h"
#include "Object.h"

const int windowWidth = 1280;
const int windowHeight = 720;

int main(int argc, char** argv) 
{
	//Create screen objects
	std::vector<shaderObject> objects = std::vector<shaderObject>(37);

	for (size_t x = 0; x < 6; x++)
	{
		for (size_t y = 0; y < 6; y++)
		{
			objects[1 + y * 6 + x].setSphere(3.0*x, 2.0, 3.0*y, 1);
			objects[1 + y * 6 + x].mat.setColor(0.1, 0.1, 0.7);
			objects[1 + y * 6 + x].mat.reflectivity = 0.9;
		}
	}

	objects[0].setBox(-25.0, -0.5, -25.0, 25.0, -0.4, 25.0);
	objects[0].mat.setColor(0.3, 0.3, 0.3);


	//Renderer setup
	bool quit = false;

	Renderer renderer(objects, windowWidth, windowHeight);
	InputHandler inputHandler(renderer, renderer.camera);

	int frameCount = 0;
	
	renderer.renderType = renderer.PathTracer;
	renderer.init();

	//3D Mesh example
	/*
	renderer.renderType = renderer.Rasterizer;
	renderer.init();

	std::vector<float> headVertices = FileHandler::loadObj("teapot.obj");
	Mesh head = Mesh(Vec3i{ 0, 0, 0 }, headVertices);

	std::vector<Mesh> meshes = std::vector<Mesh>();
	meshes.push_back(head);

	renderer.loadVBOs(meshes); */

	while (!quit) {
		if (inputHandler.windowContext) {
			renderer.render();
			if (frameCount % 180 == 0)
			{
				std::cout << "Frame took " << renderer.deltaTime << " ms (" << 1000 / renderer.deltaTime << " fps)" << std::endl;
			}

			frameCount++;
		}
		if (inputHandler.exit)
		{
			quit = true;
		}
		inputHandler.handleInput(renderer.deltaTime);
		renderer.updateDeltatime();
	}

	renderer.close();
	return 0;
}
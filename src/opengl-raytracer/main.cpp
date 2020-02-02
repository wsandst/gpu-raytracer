#include <iostream>
#include <vector>

#define NOMINMAX
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "stb_image_write.h"

#include "Renderer.h"
#include "InputHandler.h"
#include  <iostream>
#include <ctime>

const int mWidth = 1024;
const int mHeight = 1024;

int main(int argc, char** argv) 
{
	bool quit = false;

	InputHandler inputHandler;
	Renderer renderer;

	int frameCount = 0;

	//Screen texture example
	
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
		inputHandler.handleInput(renderer.camera, renderer.deltaTime);
		renderer.updateDeltatime();
	}

	renderer.close();
	return 0;
}
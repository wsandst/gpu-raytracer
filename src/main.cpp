#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>

#define NOMINMAX
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "stb_image_write.h"

#include "Renderer.h"
#include "InputHandler.h"
#include "Object.h"
#include "Scene.h"

const int windowWidth = 1280;
const int windowHeight = 720;

int main(int argc, char** argv) 
{
	//Create screen objects
	Scene scene;
	scene.addBox(glm::vec3 {-25.0, -1.5, -25.0}, glm::vec3 {50.0, 0.1, 50.0}, glm::vec4 {0.3, 0.3, 0.3, 1.0});
	scene.addSphere(glm::vec3 {10.0, 2.0, 3.0}, 1, glm::vec4 {0.1, 0.1, 0.7, 1.0}, 0.7);
	scene.addMesh("cube.obj", glm::vec3 {7.0, 2.0, 10.0}, glm::vec4 {1, 0.1, 0.1, 1.0}, 2, 0.7, 1.2);
	scene.addMesh("head.obj", glm::vec3 {3.0, 2.0, 10.0}, glm::vec4 {1, 0.1, 0.1, 1.0});

	//Renderer setup
	bool quit = false;

	Renderer renderer(scene.objects, scene.vertices, scene.normals, windowWidth, windowHeight);
	InputHandler inputHandler(renderer, renderer.camera);

	int frameCount = 0;
	
	renderer.renderType = renderer.PathTracer;
	renderer.init();

	auto begin = std::chrono::system_clock::now();

	//Main render/event loop
	while (!quit) {
		if (inputHandler.windowContext) {
			renderer.render();

			//FPS counter
			auto now = std::chrono::system_clock::now();
			if (std::chrono::duration_cast<std::chrono::milliseconds>(now - begin).count() > 2000)
			{
				std::cout << "Frame took " << renderer.deltaTime << " ms (" << 1000 / renderer.deltaTime << " fps)" << "\n";
				begin = std::chrono::system_clock::now();
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
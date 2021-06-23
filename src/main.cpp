#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <memory>

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

	// 36 reflective spheres
	//scene.addBox(glm::vec3 {-25.0, -1.5, -25.0}, glm::vec3 {50.0, 0.1, 50.0}, glm::vec4 {0.3, 0.3, 0.3, 1.0});
	/*for (size_t i = 0; i < 36; i++)
	{
		float x = (i % 6) * 3;
		float y = (i / 6) * 3;
		std::cout << x << " " << y << std::endl;
		scene.addSphere(glm::vec3 {x, 0.6, y}, 1, glm::vec4 {0.1, 0.1, 0.7, 1.0}, 0, 1.0, 1);
	}*/


	// Multiple lights and shadows example
	//scene.addBox(glm::vec3 {-25.0, -1.5, -25.0}, glm::vec3 {50.0, 0.1, 50.0}, glm::vec4 {0.3, 0.3, 0.3, 1.0});
	//scene.addBox(glm::vec3 {-25.0, -1.4, 25.0}, glm::vec3 {50.0, 25.0, 0.1}, glm::vec4 {0.3, 0.3, 0.3, 1.0});
	//scene.addSphere(glm::vec3 {1.0, 5.0, 1.0}, 0.5, glm::vec4 {1, 0.8, 0.6, 1.0}, 0.95); //Represents the point light source
	//scene.addSphere(glm::vec3 {10.0, 2.0, 3.0}, 1, glm::vec4 {0.1, 0.1, 0.7, 1.0});
	//scene.addSphere(glm::vec3 {20.0, 5.0, 10.0}, 3, glm::vec4 {0.1, 0.1, 0.7, 1.0});
	//scene.addSphere(glm::vec3 {0.0, 2.0, 7.0}, 2, glm::vec4 {0.1, 0.1, 0.7, 1.0});
	//  Lights
	//scene.addLightDirectional(glm::normalize(glm::vec3{1.0, 1.0, 1.0}), glm::vec3(0.5));
	//scene.addLightPoint(glm::vec3 {1.0, 5.0, 1.0}, 100.0, glm::vec3 {1.0, 1.0, 1.0});
	//scene.addSphere(glm::vec3 {5.0, 1.3, 2.0}, 3, glm::vec4 {0.9, 0.1, 0.1, 1.0}, 0.6, 1.4, 0);

	// Refraction sphere
	//scene.addSphere(glm::vec3 {0, 0, 0}, 3, glm::vec4 {1.0, 0.1, 0.1, 1.0}, 0.7, 1.5, 1.2);
	//scene.addSphere(glm::vec3 {10.0, 0, 0}, 1, glm::vec4 {0.1, 0.1, 0.7, 1.0});
	//scene.addMesh("cube.obj", glm::vec3 {7.0, 2.0, 10.0}, glm::vec4 {1, 0.1, 0.1, 1.0}, 2, 0.7, 1.2);
	
	// Mirrors pointed to each other
	//scene.addBox(glm::vec3 {-25.0, -1.4, 25.0}, glm::vec3 {50.0, 25.0, 0.1}, glm::vec4 {0.3, 0.3, 0.3, 1.0}, 1.0, 0.0, 1.0, 0.95);
	//scene.addBox(glm::vec3 {-25.0, -1.4, 0.0}, glm::vec3 {50.0, 25.0, 0.1}, glm::vec4 {0.3, 0.3, 0.3, 1.0}, 1.0, 0.0, 1.0, 0.95);

	// Cornell box scene, 1 light
	scene.addLightPoint(glm::vec3 {5.0, 9.0, 3.5}, 50.0, glm::vec3 {1.0, 1.0, 0.8});
	//scene.addSphere(glm::vec3 {5.0, 9.0, 5.0}, 0.5, glm::vec4 {1, 0.8, 0.6, 1.0}, 0.95); //Represents the point light source

	// Box
	scene.addBox(glm::vec3 {0.0, 0, 0.0}, glm::vec3 {10.0, 0.1, 6.9}, glm::vec4 {1.0, 1.0, 1.0, 1.0}); //Bottom, white
	scene.addBox(glm::vec3 {0.0, 9.9, 0.0}, glm::vec3 {10.0, 0.1, 6.9}, glm::vec4 {1.0, 1.0, 1.0, 1.0}); //Top, white
	scene.addBox(glm::vec3 {0.0, 0, 0.0}, glm::vec3 {0.1, 10.0, 6.9}, glm::vec4 {1.0, 1.0, 1.0, 1.0}); //Back, white
	scene.addBox(glm::vec3 {0.0, 0, 6.9}, glm::vec3 {10, 10.0, 0.1}, glm::vec4 {1.0, 0, 0, 1.0}); //Left, red
	scene.addBox(glm::vec3 {0.0, 0, 0}, glm::vec3 {10, 10.0, 0.1}, glm::vec4 {0, 1.0, 0, 1.0}); //Right, green

	// Contents
	// Reflection example
	scene.addSphere(glm::vec3 {5.0, 1.3, 2.0}, 1.2, glm::vec4 {0.5, 0.5, 0.5, 1.0}, 0.0, 1.0, 0.4);
	scene.addSphere(glm::vec3 {5.0, 1.3, 5.0}, 1.2, glm::vec4 {0.5, 0.5, 0.5, 1.0}, 0.0, 1.0, 0.4);
	// Mesh examples
	//scene.addMesh("teapot2.obj", glm::vec3 {3.0, 0.1, 3.0}, glm::vec4 {1, 0.1, 0.1, 1.0}, 0.03);
	//scene.addMesh("head.obj", glm::vec3 {3.5, 1.0, 1.75}, glm::vec4 {0.3, 0.3, 0.3, 1}, 1); //Head
	// Fresnel example
	//scene.addBox(glm::vec3 {10, 0, 0.0}, glm::vec3 {0.1, 10.0, 6.9}, glm::vec4 {1.0, 0.9, 0.9, 1.0}, 1.0, 0.9, 1.2); //Window to Cornell Box
	// Refraction example
	//scene.addBox(glm::vec3 {2.5, 0.1, 3.0}, glm::vec3 {2.0, 2.0, 1.5}, glm::vec4 {1.0, 0.9, 0.9, 1.0}, 1.0, 0.9, 1.3); //Glass box
	//scene.addMesh("rotatedrod.obj", glm::vec3 {3.5, 0, 1.75}, glm::vec4 {0.3, 0.3, 0.3, 1}, 0.025); //Rotated rod

	//Renderer setup
	bool quit = false;

	Renderer renderer(scene, windowWidth, windowHeight);
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
				//Modifying object test
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
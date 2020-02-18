#pragma once
#include "Camera.h"
#include "Renderer.h"
#include "SDL.h"
class InputHandler
{
private:
	SDL_Event sdlEvent;
public:
	float sensitivity = 0.5;
	bool exit = false;
	bool windowContext = true;

	Camera& camera;
	Renderer& renderer;

	void handleInput(float deltaTime);
	InputHandler(Renderer& _renderer, Camera& _camera);
	~InputHandler();
};


#pragma once
#include "Camera.h"
#include "SDL.h"
class InputHandler
{
private:
	SDL_Event sdlEvent;
public:
	float sensitivity = 0.5;
	bool exit = false;
	bool windowContext = true;

	int currentImage;

	void handleInput(Camera &camera, float deltaTime);
	InputHandler();
	~InputHandler();
};


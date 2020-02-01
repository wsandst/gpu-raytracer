#include "InputHandler.h"

int count = 0;

void InputHandler::handleInput(Camera &camera, float deltaTime)
{
	camera.cameraStep = 0.05f * deltaTime;
	float xOffset, yOffset;
	while (SDL_PollEvent(&sdlEvent) != 0) {
		switch (sdlEvent.type) {
		case SDL_KEYDOWN:
			switch (sdlEvent.key.keysym.sym) {
			case SDLK_w:
				camera.moveForward();
				break;
			case SDLK_s:
				camera.moveBack();
				break;
			case SDLK_a:
				camera.moveLeft();
				break;
			case SDLK_d:
				camera.moveRight();
				break;
			case SDLK_SPACE:
				camera.moveUp();
				break;
			case SDLK_LCTRL:
				camera.moveDown();
				break;
			case SDLK_F9:
				std::cout << "x: " << camera.getPosition().x << " y: " << camera.getPosition().y << " z: " << camera.getPosition().z << "\n";
				break;
				
			case SDLK_F10:
				currentImage = count % 3;
				count++;
				break;
				
			case SDLK_ESCAPE:
				windowContext = !windowContext;
				if (windowContext) {
					SDL_ShowCursor(0);
					SDL_SetRelativeMouseMode(SDL_TRUE);
				}
				else {
					SDL_ShowCursor(1);
					SDL_SetRelativeMouseMode(SDL_FALSE);
				}
				break;
			case SDLK_c:
				exit = true;
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			int relativeX, relativeY;
			SDL_GetRelativeMouseState(&relativeX, &relativeY);

			camera.updateView(relativeX * sensitivity, -relativeY * sensitivity);
			break;
		}
	}
}

InputHandler::InputHandler()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
}


InputHandler::~InputHandler()
{
}

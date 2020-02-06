#include "InputHandler.h"

int count = 0;

void InputHandler::handleInput(float deltaTime)
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
				
			case SDLK_F11:
				renderer.toggleFullscreen();
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
			if (windowContext)
			{
				int relativeX, relativeY;
				SDL_GetRelativeMouseState(&relativeX, &relativeY);

				camera.updateView(relativeX * sensitivity, -relativeY * sensitivity);
			}
			break;
		case SDL_WINDOWEVENT:
			switch (sdlEvent.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
					renderer.resizeWindow(sdlEvent.window.data1, sdlEvent.window.data2);
					printf("Window size changed to %dx%d \n",
						sdlEvent.window.data1,
						sdlEvent.window.data2);
					break;
			}
			break;
		}
	}
}

InputHandler::InputHandler(Renderer& _renderer, Camera& _camera) : camera(_camera), renderer(_renderer)
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
}


InputHandler::~InputHandler()
{
}

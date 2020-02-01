#pragma once
#define GLEW_STATIC

#include "GL/glew.h"

#include "SDL.h"
#include "SDL_opengl.h"

#include "glm/glm.hpp"

#include "Shader.h"
#include "Camera.h"
#include "VBOWrapper.h"
#include "Mesh.h"

#include <vector>
#include <unordered_map>

class Renderer {
public:
	const char* windowTitle = "C++ OpenGL Boilerplate";
    const int windowWidth = 1024, windowHeight = 1024;
	int screenHeight, screenWidth;

	enum RenderType {ScreenTexture, Geometry};
	RenderType renderType = ScreenTexture;

    float deltaTime = 0.0f;
    Camera camera;

    Renderer();

    ~Renderer();

    void updateDeltatime();

    void render();

	void drawGeometry();

	void initGeometry();

	//Functions related to drawing a screen texture from a byte array
	void drawScreenTexture();

    void updateScreenTexture(unsigned char *byteArray, int width = 1024, int height = 1024);

    void initScreenTexture();

    void init();

    void close();

	void loadVBOs(std::vector<Mesh>& meshes);

private:
    Shader screenTextureShader, geometryShader;
    SDL_Window *window;
	std::vector<GeometryVBO> geometryVBOs;

    SDL_GLContext glContext;
    unsigned int VAO, VBO, texture;
    float lastFrame = 0.0f;

    void initOpenGL();

    void initSDL();

	void resizeWindow(int width, int height);
};


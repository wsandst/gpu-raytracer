#pragma once
#define GLEW_STATIC

#include "GL/glew.h"

#include "SDL.h"
#include "SDL_opengl.h"

#include "glm/glm.hpp"

#include "Shader.h"
#include "ComputeShader.h"
#include "Camera.h"
#include "VBOWrapper.h"
#include "Mesh.h"

#include <vector>
#include <unordered_map>

class Renderer {
public:
	const char* windowTitle = "OpenGL Raytracer using Compute Shaders";
    const int windowWidth = 1024, windowHeight = 1024;
	int screenHeight, screenWidth;

	enum RenderType {PathTracer, Rasterizer};
	RenderType renderType = PathTracer;

    float deltaTime = 0.0f;
    Camera camera;

    Renderer();

    ~Renderer();

    void updateDeltatime();

    void render();

	void drawRasterizer();

	void initRasterizer();

	//Functions related to drawing a screen texture from a byte array
	void drawPathTracer();

    void updateScreenTexture(unsigned char *byteArray, int width = 1024, int height = 1024);

    void initPathTracer();

    void init();

    void close();

	void loadVBOs(std::vector<Mesh>& meshes);

private:
    Shader screenTextureShader, rasterizerShader;
	ComputeShader pathTracerComputeShader;
    SDL_Window *window;
	std::vector<GeometryVBO> geometryVBOs;

    SDL_GLContext glContext;
    unsigned int VAO, VBO, textureOutput;
    float lastFrame = 0.0f;

    void initOpenGL();

    void initSDL();

	void resizeWindow(int width, int height);
};


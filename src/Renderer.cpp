#include "Renderer.h"


Renderer::Renderer(Scene& _scene, int windowWidth, int windowHeight) : 
scene(_scene)
{
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
}

Renderer::~Renderer()
{
}

void Renderer::updateDeltatime()
{
	float currentFrame = SDL_GetTicks();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void Renderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera.calculateProjectionMatrix();
	camera.calculateViewMatrix();
	camera.getCornerRays();

	switch (renderType)
	{
		case PathTracer: drawPathTracer(); break;
		case Rasterizer: drawRasterizer(); break;
	}

	SDL_GL_SwapWindow(window);
}

void Renderer::drawRasterizer()
{
	rasterizerShader.use();

	for (auto& it : geometryVBOs) { //Draw chunks
		glm::mat4 matrix = glm::mat4(camera.getProjectionMatrix() * camera.getViewMatrix() * it.translation);
		rasterizerShader.setMat4("matrix", matrix);
		glBindVertexArray(it.VAO);
		glDrawArrays(GL_TRIANGLES, 0, it.size);
	}
}

void Renderer::initRasterizer()
{
	rasterizerShader = Shader("shaders/geometry.vert", "shaders/geometry.frag");
}

void Renderer::drawPathTracer()
{
	pathTracerComputeShader.use();

	int framebufferTexLocation = glGetUniformLocation(pathTracerComputeShader.ID, "framebuffer");
	int skyBoxTexLocation = glGetUniformLocation(pathTracerComputeShader.ID, "skybox");
	int vertexBufferLocation = glGetUniformLocation(pathTracerComputeShader.ID, "objectVertexBuffer");
	int normalBufferLocation = glGetUniformLocation(pathTracerComputeShader.ID, "objectNormalBuffer");
	glUniform1i(framebufferTexLocation, 0);
	glUniform1i(skyBoxTexLocation, 1);
	glUniform1i(vertexBufferLocation, 2);
	glUniform1i(normalBufferLocation, 3);
	//Set corner ray uniforms to give the program the view
	pathTracerComputeShader.setVec3("eye", camera.getPosition());
	pathTracerComputeShader.setVec3("ray00", camera.ray00);
	pathTracerComputeShader.setVec3("ray10", camera.ray10);
	pathTracerComputeShader.setVec3("ray01", camera.ray01);
	pathTracerComputeShader.setVec3("ray11", camera.ray11);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureOutput);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_BUFFER, vertexBufferTex);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_BUFFER, normalBufferTex);

	 // launch compute shaders!
	glDispatchCompute((GLuint)windowWidth*MSAALevel, (GLuint)windowHeight*MSAALevel, 1);
	
	// make sure writing to image has finished before read
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glActiveTexture(GL_TEXTURE0);
	screenTextureShader.use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}

void Renderer::init()
{
	camera = Camera(windowWidth*MSAALevel, windowHeight*MSAALevel);

	initSDL();
	initOpenGL();
	switch (renderType)
	{
	case PathTracer: initPathTracer(); break;
	case Rasterizer: initRasterizer(); break;
	}
}

void Renderer::initOpenGL()
{
	glewExperimental = GL_TRUE;
	glewInit();
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Wireframes
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glViewport(0, 0, windowWidth, windowHeight);
}

void Renderer::initSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO), 0) {
		std::cout << "SDL could not initialize! SDL_Error:" << SDL_GetError() << std::endl;
	}

	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	screenResHeight = DM.h;
	screenResWidth = DM.w;

	//Antialiasing
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, MSAALevel*MSAALevel);
	glEnable(GL_MULTISAMPLE);

	window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, 
	SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);

	glContext = SDL_GL_CreateContext(window);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	SDL_GL_SetSwapInterval(1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1); 

	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_ShowCursor(0);
}

void Renderer::initPathTracer()
{
	//Screen Texture
	//stbi_set_flip_vertically_on_load(true);
	screenTextureShader = Shader("shaders/screentexture.vert", "shaders/screentexture.frag");
	pathTracerComputeShader = ComputeShader("shaders/pathtracer.comp");

	//Compute shader
	// dimensions of the image
	glGenTextures(1, &textureOutput);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureOutput);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, windowWidth*MSAALevel, windowHeight*MSAALevel, 0, GL_RGBA, GL_FLOAT,
		NULL);
	glBindImageTexture(0, textureOutput, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	initSkyBox();

	//Object vertices samplerBuffer
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_TEXTURE_BUFFER, vertexBuffer);
	glBufferData(GL_TEXTURE_BUFFER, sizeof(float)*scene.vertices.size(), &scene.vertices[0], GL_STATIC_DRAW);

	glGenTextures(1, &vertexBufferTex);
	glBindTexture(GL_TEXTURE_BUFFER, vertexBufferTex);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F_ARB, vertexBuffer);

	//Object normals samplerBuffer
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_TEXTURE_BUFFER, normalBuffer);
	glBufferData(GL_TEXTURE_BUFFER, sizeof(float)*scene.normals.size(), &scene.normals[0], GL_STATIC_DRAW);

	glGenTextures(1, &normalBufferTex);
	glBindTexture(GL_TEXTURE_BUFFER, normalBufferTex);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F_ARB, normalBuffer);


	//Load objects
	loadObjects();
}

void Renderer::initSkyBox()
{
	stbi_set_flip_vertically_on_load(false);
	std::vector <Texture> skyBoxTextures = { FileHandler::loadImage("skybox/front.tga"), FileHandler::loadImage("skybox/back.tga"), FileHandler::loadImage("skybox/up.tga"), FileHandler::loadImage("skybox/down.tga"),
		FileHandler::loadImage("skybox/right.tga"), FileHandler::loadImage("skybox/left.tga") };
	glGenTextures(1, &skyboxTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

	for (unsigned int i = 0; i < skyBoxTextures.size(); i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_SRGB, skyBoxTextures[i].width, skyBoxTextures[i].height, 0, GL_RGB, GL_UNSIGNED_BYTE, skyBoxTextures[i].data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Renderer::requestShaderReload()
{
	switch (renderType)
	{
	case PathTracer: pathTracerComputeShader.reload(); break;
	case Rasterizer: rasterizerShader.reload(); break;
	}
}

void Renderer::close()
{
	SDL_Quit();
}

void Renderer::loadObjects()
{
	//Send objects into shader
	pathTracerComputeShader.use();
	pathTracerComputeShader.setInt("objectCount", scene.objects.size());
	pathTracerComputeShader.setInt("lightCount", scene.lights.size());
	pathTracerComputeShader.setObjects("objects", scene.objects);
	pathTracerComputeShader.setLights("lights", scene.lights);
}

void Renderer::toggleFullscreen() {

	isFullScreen = !isFullScreen;

	if (isFullScreen)
	{
		minimizedWidth = windowWidth;
		minimizedHeight = windowHeight;
		SDL_SetWindowSize(window, screenResWidth, screenResHeight);
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		windowWidth = screenResWidth;
		windowHeight = screenResHeight;

	}
	else
	{
		SDL_SetWindowFullscreen(window, 0);
		SDL_SetWindowSize(window, minimizedWidth, minimizedHeight);
		windowWidth = minimizedWidth;
		windowHeight = minimizedHeight;
	}

	updateResolution();
}

void Renderer::updateResolution()
{

	glViewport(0, 0, windowWidth, windowHeight);

	glActiveTexture(GL_TEXTURE0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, windowWidth*MSAALevel, windowHeight*MSAALevel, 0, GL_RGBA, GL_FLOAT,
		NULL);
	
	camera.windowWidth = windowWidth*MSAALevel;
	camera.windowHeight = windowHeight*MSAALevel;
	render();

}

void Renderer::resizeWindow(int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	updateResolution();
}

///@brief Make a screenshot of the screen and save it to a file in the executable directory, named after the current date and time
void Renderer::screenshot()
{
	//Get timestamp string
	std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);

	std::string date = std::to_string(now->tm_year + 1900)
         + (now->tm_mon < 9 ? "0" : "") + std::to_string(now->tm_mon + 1)
         + (now->tm_mday < 10 ? "0" : "") + std::to_string(now->tm_mday)
		 + "_"
		 + (now->tm_hour < 10 ? "0" : "") + std::to_string(now->tm_hour)
		 + (now->tm_min < 10 ? "0" : "") + std::to_string(now->tm_min)
		 + (now->tm_sec < 10 ? "0" : "") + std::to_string(now->tm_sec)
		 + ".png";

	///OpenGL screen is flipped vertically
	stbi_flip_vertically_on_write(1);

	//Retrieve a bitmap array of the screen from OpeNGL
	GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    int x = viewport[0];
    int y = viewport[1];
    int width = viewport[2];
    int height = viewport[3];

    std::vector<char> data(width * height * 3);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);

	//Write the bitmap array to file as png
    int saved = stbi_write_png(date.data(), width, height, 3, &data[0], 0);

	std::cout << "Saved screenshot as " << date << "\n";

}
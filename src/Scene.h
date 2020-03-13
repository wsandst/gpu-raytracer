#pragma once
#include <vector>

#include "SDL.h"
#include "SDL_opengl.h"
#include "glm/glm.hpp"

#include "FileHandler.h"
#include "Object.h"

class Scene
{
private:
public:
    std::vector<shaderObject> objects = std::vector<shaderObject>();
    std::vector<GLfloat> vertices = std::vector<GLfloat>();
    std::vector<GLfloat> normals = std::vector<GLfloat>();

    void addSphere(glm::vec3 pos, float radius, glm::vec4 color, float transparency = 0, float refractivity = 1, float reflectivity = 0)
    {
        shaderObject object;
        object.setSphere(pos, radius);
        object.mat.setColor(color);
        object.mat.transparency = transparency, object.mat.reflectivity = reflectivity, object.mat.refractivity = refractivity;
        objects.push_back(object);
    }

    //Add box
    void addBox(glm::vec3 pos, glm::vec3 bBox, glm::vec4 color, float scale = 1, float transparency = 0, float refractivity = 1, float reflectivity = 0)
    {
        shaderObject object;
        object.setBox(pos, bBox, {0,0,0}, scale);
        object.mat.setColor(color);
        object.mat.transparency = transparency, object.mat.reflectivity = reflectivity, object.mat.refractivity = refractivity;
        objects.push_back(object);
    }

    //Add mesh
	void addMesh(std::string name, glm::vec3 pos, glm::vec4 color, float scale = 1, float transparency = 0, float refractivity = 1, float reflectivity = 0)
    {
        shaderObject object;

        //Load .obj file
        glm::vec3 min {1000,1000,1000}, max {-1000, -1000, -1000};
        std::vector<GLfloat> objVertices = std::vector<GLfloat>();
	    std::vector<GLfloat> objNormals = std::vector<GLfloat>();
        FileHandler::loadObj(name, objVertices, objNormals, min, max);
        //Combien loaded vertices with the total scene vertices which are sent into the compute shader
        int vertexCount = vertices.size() / 3;
        object.setMesh(pos, vertexCount, vertexCount + objVertices.size() / 3, max, min, scale);
        vertices.insert(vertices.end(), objVertices.begin(), objVertices.end());
        normals.insert(normals.end(), objNormals.begin(), objNormals.end());

        object.mat.setColor(color);
        object.mat.transparency = transparency, object.mat.reflectivity = reflectivity, object.mat.refractivity = refractivity;
        objects.push_back(object);
    }


    Scene(/* args */);
    ~Scene();
};

Scene::Scene(/* args */)
{
}

Scene::~Scene()
{
}

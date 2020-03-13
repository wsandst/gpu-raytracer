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

    void addSphere(glm::vec3 pos, float r, glm::vec4 color, float transparency = 0, float refractivity = 1, float reflectivity = 0)
    {
        shaderObject object;
        object.setSphere(pos, r);
        object.mat.setColor(color);
        object.mat.transparency = transparency, object.mat.reflectivity = reflectivity, object.mat.refractivity = refractivity;
        objects.push_back(object);
    }

    //Add box
    void addBox(glm::vec3 min, glm::vec3 max, glm::vec4 color, float transparency = 0, float refractivity = 1, float reflectivity = 0)
    {
        shaderObject object;
        object.setBox(min, max);
        object.mat.setColor(color);
        object.mat.transparency = transparency, object.mat.reflectivity = reflectivity, object.mat.refractivity = refractivity;
        objects.push_back(object);
    }

    //Add mesh
	void addMesh(std::string name, glm::vec3 pos, glm::vec4 color, float transparency = 0, float refractivity = 1, float reflectivity = 0)
    {
        shaderObject object;

        //Load .obj file
        glm::vec3 min, max;
        std::vector<GLfloat> objVertices = std::vector<GLfloat>();
	    std::vector<GLfloat> objNormals = std::vector<GLfloat>();
        FileHandler::loadObj(name, objVertices, objNormals, min, max);
        //Combien loaded vertices with the total scene vertices which are sent into the compute shader
        int vertexCount = vertices.size() / 3;
        object.setMesh(vertexCount, vertexCount + objVertices.size() / 3, min, max);
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

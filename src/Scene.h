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
    std::vector<shaderLight> lights = std::vector<shaderLight>();
    std::vector<shaderObject> objects = std::vector<shaderObject>();
    std::vector<GLfloat> vertices = std::vector<GLfloat>();
    std::vector<GLfloat> normals = std::vector<GLfloat>();
    std::vector<GLfloat> boundingBoxes = std::vector<GLfloat>();

    //Add a sphere
    void addSphere(glm::vec3 pos, float radius, glm::vec4 color, float transparency = 0, float refractivity = 1, float reflectivity = 0)
    {
        shaderObject object;
        object.setSphere(pos, radius);
        object.mat.setColor(color);
        object.mat.transparency = transparency, object.mat.reflectivity = reflectivity, object.mat.refractivity = refractivity;
        objects.push_back(object);
    }

    //Add a box
    void addBox(glm::vec3 pos, glm::vec3 bBox, glm::vec4 color, float scale = 1, float transparency = 0, float refractivity = 1, float reflectivity = 0)
    {
        shaderObject object;
        object.setBox(pos, bBox, {0,0,0}, scale);
        object.mat.setColor(color);
        object.mat.transparency = transparency, object.mat.reflectivity = reflectivity, object.mat.refractivity = refractivity;
        objects.push_back(object);
    }

    //Add a mesh
	void addMesh(std::string name, glm::vec3 pos, glm::vec4 color, float scale = 1, float transparency = 0, float refractivity = 1, float reflectivity = 0)
    {
        shaderObject object;

        //Load .obj file
        glm::vec3 min {1000,1000,1000}, max {-1000, -1000, -1000};
        std::vector<GLfloat> objVertices = std::vector<GLfloat>();
	    std::vector<GLfloat> objNormals = std::vector<GLfloat>();
        FileHandler::loadObj(name, objVertices, objNormals, min, max);
        //Combine loaded vertices with the total scene vertices which are sent into the compute shader
        int vertexCount = vertices.size() / 3;
        //object.setMesh(pos, vertexCount, vertexCount + objVertices.size() / 3, max, min, scale);
        object.setMesh(pos, 0, 6, max, min, scale);
        vertices.insert(vertices.end(), objVertices.begin(), objVertices.end());
        normals.insert(normals.end(), objNormals.begin(), objNormals.end());

        object.mat.setColor(color);
        object.mat.transparency = transparency, object.mat.reflectivity = reflectivity, object.mat.refractivity = refractivity;
        objects.push_back(object);


        addBoundingBox(glm::vec3 {-71.8925323, 0, -47.9283562}, glm::vec3 {82.2939987, 79.0065613, 47.9283562}, vertexCount, vertexCount + objVertices.size() / (3*2));
	    addBoundingBox(glm::vec3 {-71.8925323, 180, -47.9283562}, glm::vec3 {82.2939987, 79.0065613, 47.9283562}, vertexCount + objVertices.size() / (3*2), vertexCount + objVertices.size() / 3);
    }

    //Add a directional light source
    void addLightDirectional(glm::vec3 dir, glm::vec3 color = {1.0, 1.0, 1.0})
    {
        shaderLight light;
        light.set(LIGHT_TYPE_DIR, dir, color, 1.0);
        lights.push_back(light);
    }

    //Add a point light source
    void addLightPoint(glm::vec3 pos, float intensity = 1.0, glm::vec3 color = {1.0, 1.0, 1.0})
    {
        shaderLight light;
        light.set(LIGHT_TYPE_POINT, pos, color, intensity);
        lights.push_back(light);
    }

    void addBoundingBox(glm::vec3 min, glm::vec3 max, int vStart, int vEnd)
    {
        std::vector<GLfloat> bbBox = {min.x, min.y, min.z, 0, max.x, max.y, max.z, 0, float(vStart), float(vEnd), 0, 0};
        boundingBoxes.insert(boundingBoxes.end(), bbBox.begin(), bbBox.end());
    }

    Scene(/* args */) {}
    ~Scene() {}
};

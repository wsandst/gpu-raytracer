#pragma once
#include "SDL_opengl.h"
#include "glm/glm.hpp"

#define TYPE_MESH 0
#define TYPE_SPHERE 1
#define TYPE_BOX 2

struct shaderMaterial
{
    GLfloat color[4] = {0.5,0.5,0.5,1.0};
    GLfloat reflectivity = 0;
    GLfloat refractivity = 1;
    GLfloat transparency = 0;
    void setColor(glm::vec4 c)
    {
        color[0] = c.r, color[1] = c.g, color[2] = c.b, color[3] = c.a;
    }
};

struct shaderObject //Struct to be sent in to the compute shader
{
    GLint type;
    GLfloat pos[3] = {0,0,0};
    shaderMaterial mat = shaderMaterial();
    //Mesh
    GLint vStart = 0;
    GLint vEnd = 0;
    //Spheres
    GLfloat scale = 1; //Radius for spheres, scaling factor for everything else
    //Boxes
    GLfloat bMax[3] = {0,0,0}; //Bounding box max: width, height, depth
    GLfloat bMin[3] = {0,0,0}; //Bounding box min: width, height, depth
    void setSphere(glm::vec3 pos, float radius)
    {
        this->type = TYPE_SPHERE;
        this->pos[0] = pos.x;
        this->pos[1] = pos.y;
        this->pos[2] = pos.z;
        this->scale = radius;
    }
    void setBox(glm::vec3 pos, glm::vec3 bMax, glm::vec3 bMin = {0,0,0}, float scale = 1)
    {
        type = TYPE_BOX;
        this->pos[0] = pos.x, this->pos[1] = pos.y, this->pos[2] = pos.z;
        this->bMax[0] = bMax.x, this->bMax[1] = bMax.y, this->bMax[2] = bMax.z;
        this->bMin[0] = bMin.x, this->bMin[1] = bMin.y, this->bMin[2] = bMin.z;
        this->scale = scale;
    }
    void setMesh(glm::vec3 pos, int vStart, int vEnd, glm::vec3 bMax, glm::vec3 bMin = {0,0,0}, float scale = 1)
    {
        setBox(pos, bMax, bMin); //Set bounding box
        type = TYPE_MESH;
        this->vStart = vStart;
        this->vEnd = vEnd;
        this->scale = scale;
    }
};
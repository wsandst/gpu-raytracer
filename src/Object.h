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
    GLfloat pos[3];
    shaderMaterial mat = shaderMaterial();
    //Mesh
    GLint vStart = 0;
    GLint vEnd = 0;
    //Spheres
    GLfloat radius = 0;
    //Boxes
    GLfloat min[3] = {0,0,0};
    GLfloat max[3] = {0,0,0};
    void setSphere(glm::vec3 pos, float radius)
    {
        this->type = TYPE_SPHERE;
        this->pos[0] = pos.x;
        this->pos[1] = pos.y;
        this->pos[2] = pos.z;
        this->radius = radius;
    }
    void setBox(glm::vec3 min, glm::vec3 max)
    {
        type = TYPE_BOX;
        this->min[0] = min.x, this->min[1] = min.y, this->min[2] = min.z;
        this->max[0] = max.x, this->max[1] = max.y, this->max[2] = max.z;
    }
    void setMesh(int vStart, int vEnd, glm::vec3 min, glm::vec3 max)
    {
        setBox(min, max); //Set bounding box
        type = TYPE_MESH;
        this->vStart = vStart;
        this->vEnd = vEnd;
    }
};
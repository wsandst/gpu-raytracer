#pragma once
#include "SDL_opengl.h"

#define TYPE_MESH 0
#define TYPE_SPHERE 1
#define TYPE_BOX 2

struct shaderMaterial
{
    GLfloat color[4] = {0.5,0.5,0.5,1.0};
    GLfloat reflectivity = 0;
    GLfloat refractivity = 1;
    GLfloat transparency = 0;
    void setColor(float r, float g, float b, float a=1)
    {
        color[0] = r, color[1] = g, color[2] = b, color[3] = a;
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
    void setSphere(float x, float y, float z, float radius)
    {
        type = TYPE_SPHERE;
        this->pos[0] = x;
        this->pos[1] = y;
        this->pos[2] = z;
        this->radius = radius;
    }
    void setBox(const float* min, const float* max)
    {
        type = TYPE_BOX;
        this->min[0] = min[0], this->min[1] = min[1], this->min[2] = min[2];
        this->max[0] = max[0], this->max[1] = max[1], this->max[2] = max[2];
    }
    void setBox(float mina, float minb, float minc, float maxa, float maxb, float maxc)
    {
        type = TYPE_BOX;
        this->min[0] = mina, this->min[1] = minb, this->min[2] = minc;
        this->max[0] = maxa, this->max[1] = maxb, this->max[2] = maxc;
    }
    void setMesh(int vStart, int vEnd)
    {
        type = TYPE_MESH;
        this->vStart = vStart;
        this->vEnd = vEnd;
    }
};
#include "BVH.h"

BVH::BVH(std::vector<GLfloat>& vertices)
{
    for (size_t i = 0; i < vertices.size(); i += 4)
    {
        glm::vec3 v;
        v.x = vertices[i];
        v.y = vertices[i+1];
        v.z = vertices[i+2];
        vertices.push_back()
    }
}

BVH::~BVH()
{
}
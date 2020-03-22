#pragma once
#include "SDL.h"
#include "SDL_opengl.h"
#include "glm/glm.hpp"

#include <vector>


//Class for diving a mesh into one level of bounding boxes.
//Needed to improve performance on meshes in the compute shader

class BVH
{
private:
    struct BoundingBox
    {
        glm::vec3 min, max;
        int vStart, vEnd;
    };

    std::vector<glm::vec3> vertices();
    std::vector<BoundingBox> boundingBoxes();

public:
    std::vector<GLfloat> calculateBoundingBoxes();
    
    BVH(std::vector<GLfloat>& vertices);
    ~BVH();
};

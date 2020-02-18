#version 410 core

layout (location = 0) in vec3 inPos; //3 bytes

uniform mat4 matrix;

void main()
{
    gl_Position = matrix * vec4(float(inPos.x), float(inPos.y), float(inPos.z), 1.0f);	
}
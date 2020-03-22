#pragma once
#include "stb_image.h"
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <unordered_map>
#include <algorithm>

struct Texture
{
	int ID, width, height, nrChannels;
	unsigned char* data;
};

class FileHandler
{
private:
	static void calcSurfaceNormal(float N[3], float v0[3], float v1[3], float v2[3]) 
	{
		float v10[3];
		v10[0] = v1[0] - v0[0];
		v10[1] = v1[1] - v0[1];
		v10[2] = v1[2] - v0[2];

		float v20[3];
		v20[0] = v2[0] - v0[0];
		v20[1] = v2[1] - v0[1];
		v20[2] = v2[2] - v0[2];

		N[0] = v20[1] * v10[2] - v20[2] * v10[1];
		N[1] = v20[2] * v10[0] - v20[0] * v10[2];
		N[2] = v20[0] * v10[1] - v20[1] * v10[0];

		float len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
		if (len2 > 0.0f) {
			float len = sqrtf(len2);

			N[0] /= len;
			N[1] /= len;
			N[2] /= len;
		}
	}
	static void normalize(float &x, float &z, float &y)
	{
		float mag = std::sqrt(x*x + z*z + y*y);
		if (mag > 0.0)
		{
			x = x / mag;
			y = y / mag;
			z = z / mag;
		}
	}
public:

	static std::string loadStrFile(const char* path)
	{

		std::ifstream file(path, std::ios::binary | std::ios::ate);
		if (!file.is_open()) {
			std::cout << "Could not read file at " << path << "!" << std::endl;
		}
		std::string source;
		source.resize(file.tellg());
		file.seekg(0, std::ios::beg);
		file.read(&source[0], source.size());

		std::cout << "Loaded file: " << path << std::endl;

		return source;
	}

	static Texture loadImage(std::string filename, int channels = 3)
	{
		Texture texture;
		std::string newFileName = "assets/textures/" + filename;
		const char* file = newFileName.c_str();
		int width, height, nrChannels;
		texture.data = stbi_load(file, &width, &height, &nrChannels, channels);
		texture.width = width;
		texture.height = height;
		texture.nrChannels = nrChannels;
		if (!texture.data)
			std::cout << "Failed to load texture: " << filename << std::endl;
		else
			std::cout << "Loaded texture: " << filename << " (w:" << width << ", h:" << height << ")" << std::endl;
		return texture;
	}

	static void loadObj(std::string filename, std::vector<float>& vertices, std::vector<float>& normals, glm::vec3& min, glm::vec3& max)
	{
		//Loads an obj file from disk using tinyobjloader. Generates smooth vertex normals if no normals are specified in file.
		//Also creates a box for the object, specified in min and max.
		std::string inputfile = "assets/models/" + filename;
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string warn;
		std::string err;

		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str());

		if (!warn.empty()) {
			std::cout << warn << std::endl;
		}

		if (!err.empty()) {
			std::cerr << err << std::endl;
		}

		if (!ret) {
			exit(1);
		}

		// Check if the file has normals
		bool fileHasNormals = attrib.normals.size() > 0;

		std::vector<float> tempNormals; //Used to average face normals to get compued vertex normals
		for (size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces(polygon)
			tempNormals = std::vector<float>(shapes[s].mesh.indices.size());
			size_t index_offset = 0;
			for (size_t f = 0; f <  shapes[s].mesh.indices.size() / 3; f++) {
				int fv = shapes[s].mesh.num_face_vertices[f];
				tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
        		tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
				tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];
				float v[3][3];
				int f0 = idx0.vertex_index;
				int f1 = idx1.vertex_index;
				int f2 = idx2.vertex_index;
				for (int k = 0; k < 3; k++) 
				{
					v[0][k] = attrib.vertices[3 * f0 + k];
					v[1][k] = attrib.vertices[3 * f1 + k];
					v[2][k] = attrib.vertices[3 * f2 + k];
				}
				float n[3][3];
				if (fileHasNormals) //Get the normals if the file has them
				{
					int nf0 = idx0.normal_index;
					int nf1 = idx1.normal_index;
					int nf2 = idx2.normal_index;

					for (int k = 0; k < 3; k++) {
						n[0][k] = attrib.normals[3 * nf0 + k];
						n[1][k] = attrib.normals[3 * nf1 + k];
						n[2][k] = attrib.normals[3 * nf2 + k];
					}
				}
				else
				{
				//If the file has no normals, calculate them.
				calcSurfaceNormal(n[0], v[0], v[1], v[2]);
				for (size_t i = 0; i < 3; i++)
				{
					tempNormals[3 * idx0.vertex_index + i] += n[0][i];
					tempNormals[3 * idx1.vertex_index + i] += n[0][i];
					tempNormals[3 * idx2.vertex_index + i] += n[0][i];
				}
				} //Insert the relevant data in the output vector/vectors
				for (size_t i = 0; i < 3; i++)
				{
					vertices.insert(vertices.end(), { v[i][0], v[i][1], v[i][2], 0 });
					if (fileHasNormals)
						normals.insert(normals.end(), {n[i][0], n[i][1], n[i][2], 0});
				}
				//Calculate bounding box of shape.
				for (size_t i = 0; i < 3; i++)
				{
					for (size_t j = 0; j < 3; j++)
					{
						max[i] = std::max(max[i], v[j][i]);
						min[i] = std::min(min[i], v[j][i]);
					}
					
				}
				
					
			}
			//If we are computing the normals, reiterate over the vertices again to generate the normal output vector from tempNormals
			if (!fileHasNormals)
			{
			size_t index_offset = 0;
			for (size_t f = 0; f <  shapes[s].mesh.indices.size() / 3; f++) {
				int fv = shapes[s].mesh.num_face_vertices[f];
				tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
				tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
				tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];
				float n[3][3];
				for (int k = 0; k < 3; k++) 
				{
					int f0 = idx0.vertex_index;
					int f1 = idx1.vertex_index;
					int f2 = idx2.vertex_index;

					n[0][k] = tempNormals[3 * f0 + k];
					n[1][k] = tempNormals[3 * f1 + k];
					n[2][k] = tempNormals[3 * f2 + k];
				}
				float x = 0, y = 0, z = 0;
				for (size_t i = 0; i < 3; i++)
				{
					x += n[i][0], y += n[i][1], z += n[i][2];
					//normalize(n[i][0], n[i][1], n[i][2]);
					normals.insert(normals.end(), {n[i][0], n[i][1], n[i][2], 0});
				}
			}
		}
		}
		
		std::cout << "Loaded texture: " << filename << " (" << vertices.size() / (4 * 3) << " polygons)" << std::endl;
	}

	static void saveFile(std::string path, std::string file)
	{
		std::ofstream out(path);
		out << file;
		out.close();
	}

	static std::vector<std::string> split(std::string input, char delimiter)
	{
		std::vector<std::string> output;
		size_t foundSymbolAt = 0;
		size_t foundSymbolAtOld = -1;
		int i = 0;
		while (foundSymbolAt != -1)
		{
			foundSymbolAt = input.find(delimiter, (int)(foundSymbolAtOld + 1));
			output.push_back(input.substr(foundSymbolAtOld + 1, foundSymbolAt - foundSymbolAtOld - 1));
			foundSymbolAtOld = foundSymbolAt;
			i++;
		}
		return output;
	}

	FileHandler() {};
	~FileHandler() {};
};
#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "stb_image.h"
#include "tiny_obj_loader.h"

struct Texture
{
	int ID, width, height, nrChannels;
	unsigned char* data;
};

class FileHandler
{
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
		std::string newFileName = "resources/textures/" + filename;
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

	static std::vector<float> loadObj(std::string filename)
	{
		std::vector<float> vertices = std::vector<float>();
		std::string inputfile = "resources/models/" + filename;
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

		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces(polygon)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				int fv = shapes[s].mesh.num_face_vertices[f];

				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
					tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
					tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
					tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
					tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
					//vertices.insert(vertices.end(), { vx, vy, vz, tx, ty }); //With textures
					vertices.insert(vertices.end(), { vx, vy, vz }); //Without textures
				}
				index_offset += fv;
			}
		}
		return vertices;
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
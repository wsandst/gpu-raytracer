#pragma once
#include "MapWrapper.h"

enum ImageType { Height, Satellite, Tectonic};

class ImageHelper
{
public:

	unsigned char* bitmap;

	int mapWidth;
	int mapHeight;

	ImageHelper(int mapWidth, int mapHeight)
	{
		this->mapWidth = mapWidth;
		this->mapHeight = mapHeight;
		bitmap = new unsigned char[mapWidth*mapHeight * 3];
	}

	ImageHelper() {};
	~ImageHelper()
	{
		delete[] bitmap;
	};

	unsigned char* generateHeightImage(Array2Di &map)
	{
		int maxHeight = map.getMaxValue();
		int minHeight = map.getMinValue();
		float factor = 1 / ((maxHeight - minHeight) / 255.0f);
		std::cout << "Scaling factor: " << factor << std::endl;
		int heightColor;
		for (size_t i = 0; i < map.width * map.height; i++)
		{
			heightColor = (map[i] - minHeight) * factor;
			bitmap[i * 3] = heightColor;
			bitmap[i * 3 + 1] = heightColor;
			bitmap[i * 3 + 2] = heightColor;
		}
		return bitmap;
	}

	unsigned char* generateHeightImage(Array2Df& map) //Assuming a normalized map
	{
		float maxHeight = map.getMaxValue();
		float minHeight = map.getMinValue();
		float factor = 1 / (maxHeight - minHeight);
		std::cout << "Scaling factor: " << factor << std::endl;
		int heightColor;
		for (size_t i = 0; i < map.width * map.height; i++)
		{
			heightColor = ((map[i] - minHeight) * factor * 255);
			bitmap[i * 3] = heightColor;
			bitmap[i * 3 + 1] = heightColor;
			bitmap[i * 3 + 2] = heightColor;
		}
		return bitmap;
	}

	unsigned char* generateRegionImage(Array2Di &map, int regionCount)
	{
		
		for (size_t i = 0; i < map.width * map.height; i++)
		{
			if (map[i] != 0)
			{
				bitmap[i * 3] = 0;
				bitmap[i * 3 + 1] = 255 * (( regionCount - map[i]) / float(regionCount));
				bitmap[i * 3 + 2] = 255 * (map[i] / float(regionCount));;
			}
			else
			{
				bitmap[i * 3] = 255;
				bitmap[i * 3 + 1] = 255;
				bitmap[i * 3 + 2] = 255;
			}
		}
		return bitmap;
	}
};


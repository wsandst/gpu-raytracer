#pragma once
#include <functional>

struct Vec3i
{
	int x, y, z;
	size_t operator()(const Vec3i& pointToHash) const {
		size_t h1 = std::hash<double>()(pointToHash.x);
		size_t h2 = std::hash<double>()(pointToHash.y);
		size_t h3 = std::hash<double>()(pointToHash.z);
		return (h1 ^ (h2 << 1)) ^ (h3 << 2);
	};
	bool operator==(const Vec3i& other) const {
		return (x == other.x && y == other.y && z == other.z);
	};
	bool operator<(const Vec3i& rhs) const
	{
		return (x < rhs.x && y < rhs.y && z < rhs.z);
	}
	Vec3i operator*(const int rhs) const
	{
		return Vec3i{ x * rhs, y * rhs, z * rhs };
	}

};

struct Vec3f
{
	float x, y, z;
};

struct Vec3d
{
	float x, y, z;
};

namespace std {
	template<> struct hash<Vec3i>
	{
		std::size_t operator()(const Vec3i& p) const noexcept
		{
			return p(p);
		}
	};
}

#pragma once

#include <vector>

class PerlinNoise {
public:
	PerlinNoise();
	PerlinNoise(unsigned int seed);

	float noise(float x, float y, float z);
private:
	std::vector<int> p;

	static float fade(float t);
	static float lerp(float t, float a, float b);
	static float grad(int hash, float x, float y, float z);
};

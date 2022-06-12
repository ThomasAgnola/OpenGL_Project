#pragma once
#include "libs.h"

struct Color
{
	uint8_t r, g, b, a; // equivalent de unsigned char
};

struct Vertex
{
	glm::vec3 position;
	Color color;
	glm::vec2 texcoords;
	glm::vec3 normal;
};
#pragma once
#define GLEW_STATIC // necessite glew32s.lib

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <GL/wglew.h>

#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

struct Color
{
	uint8_t r, g, b, a; // equivalent de unsigned char
};

struct Vertex
{
	glm::vec3 position;
	Color color;
	glm::vec2 texcoords;
};
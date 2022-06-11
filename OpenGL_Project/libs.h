#pragma once
#define GLEW_STATIC // necessite glew32s.lib
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <stb_image.h>

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>

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
	glm::vec3 normal;
};
#pragma once
#define GLEW_STATIC // necessite glew32s.lib
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stb_image.h>

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <vector>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Vertex.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Primitives.h"
#include "Camera.h"
#include "Model.h"
#include "OBJLoader.h"

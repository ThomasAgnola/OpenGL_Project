#pragma once
#include <vector>
#include"Vertex.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Primitive
{
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

public:
	Primitive() {}
	virtual ~Primitive() {}

	//Functions

	void set(const Vertex* vertices, const unsigned nbrOfVertices, const GLuint* indices, const unsigned nbrOfIndices)	 {
		for (size_t i = 0; i < nbrOfVertices; i++)
		{
			//send vertices to a vertice vector
			this->vertices.push_back(vertices[i]);
		}

		for (size_t i = 0; i < nbrOfIndices; i++)
		{
			//send indices to a indice vector
			this->indices.push_back(indices[i]);
		}
	}
	
	//getters

	inline Vertex* getVertices() {
		return this->vertices.data();
	}
	inline GLuint* getIndices() {
		return this->indices.data();
	}
	inline const  unsigned getNbrOfVertices() {
		return this->vertices.size();
	}
	inline const  unsigned getNbrOfIndices() {
		return this->indices.size();
	}
};
class Triangle : public Primitive
{
public:
	Triangle()
		: Primitive()
	{
		Vertex vertices[] =
		{
			//Position                      //Color                         //TexCoords             //Normal
			glm::vec3(-0.5f, 0.5f, 0.f),    255, 0, 0, 255,                 glm::vec2(0.f, 0.f),    glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, -0.5f, 0.f),   0, 255, 0, 255,                 glm::vec2(0.f, 1.f),    glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, -0.5f, 0.f),    0, 0, 255, 255,                 glm::vec2(1.f, 1.f),    glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, 0.5f, 0.f),     255, 255, 0, 255,               glm::vec2(1.f, 0.f),    glm::vec3(0.f, 0.f, 1.f)
		};
		unsigned nbrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0, 1, 2, // Triangle 1
		};
		unsigned nbrOfIndices = sizeof(indices) / sizeof(GLuint);

		//Put in primitive container
		this->set(vertices, nbrOfVertices, indices, nbrOfIndices);
	}
};

class Quad : public Primitive
{
public:
	Quad()
		: Primitive()
	{
		Vertex vertices[] =
		{
			//Position                      //Color                         //TexCoords             //Normal
			glm::vec3(-0.5f, 0.5f, 0.f),    255, 0, 0, 255,                 glm::vec2(0.f, 0.f),    glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, -0.5f, 0.f),   0, 255, 0, 255,                 glm::vec2(0.f, 1.f),    glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, -0.5f, 0.f),    0, 0, 255, 255,                 glm::vec2(1.f, 1.f),    glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, 0.5f, 0.f),     255, 255, 0, 255,               glm::vec2(1.f, 0.f),    glm::vec3(0.f, 0.f, 1.f)
		};
		unsigned nbrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0, 1, 2, // Triangle 1
			0, 2, 3 // Triangle 2
		};
		unsigned nbrOfIndices = sizeof(indices) / sizeof(GLuint);

		//Put in primitive container
		this->set(vertices, nbrOfVertices, indices, nbrOfIndices);
	}
};
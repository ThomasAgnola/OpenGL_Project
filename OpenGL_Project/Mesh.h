#pragma once
#include "libs.h"
#include "GLShader.h"
#include <vector>
#include "Primitives.h"

class Mesh
{
private:
	int position_location;
	int color_location;
	int tex_coord;
	int normal_location;

	Vertex* vertexArray;
	unsigned nbrOfVertices;
	GLuint* indexArray;
	unsigned nbrOfIndices;

	GLuint VAO;
	GLuint VBO;
	GLuint IBO;

	glm::vec3 Position;
	glm::vec3 Origin;
	glm::vec3 Rotation;
	glm::vec3 Scale;
	glm::mat4 ModelMatrix;

	void initVAO()
	{

		// VAO, VBO, IBO
		// GEN VAO and BIND
		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		// GEN VBO and BIND/SEND DATA
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->nbrOfVertices * sizeof(Vertex), this->vertexArray, GL_STATIC_DRAW);

		// GEN IBO and BIND/SEND DATA
		if (this->nbrOfIndices > 0)
		{
			glGenBuffers(1, &this->IBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nbrOfIndices * sizeof(GLuint), this->indexArray, GL_STATIC_DRAW);
		}

		const uint32_t stride = sizeof(Vertex);
		glEnableVertexAttribArray(this->position_location);
		glVertexAttribPointer(this->position_location, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, position));

		glEnableVertexAttribArray(this->color_location);
		glVertexAttribPointer(this->color_location, 4, GL_UNSIGNED_BYTE, /*normalisation entre [0;1]*/true, stride, reinterpret_cast<void*>(offsetof(Vertex, color)));

		glEnableVertexAttribArray(this->tex_coord);
		glVertexAttribPointer(this->tex_coord, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, texcoords));

		glEnableVertexAttribArray(this->normal_location);
		glVertexAttribPointer(this->normal_location, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, normal));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void updateUniforms(GLuint& program)
	{
		glUniformMatrix4fv(glGetUniformLocation(program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(this->ModelMatrix));
	}

	void updateModelMatrix()
	{
		this->ModelMatrix = glm::mat4(1.f);
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->Origin);
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->Rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->Rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->Rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->Position - this->Origin);
		this->ModelMatrix = glm::scale(this->ModelMatrix, this->Scale);
	}

public:
	Mesh(Vertex* vertexArray,
		const unsigned& nbrOfVertices,
		GLuint* indexArray,
		const unsigned& nbrOfindices,
		int position,
		int color,
		int tex_coords,
		int normal,
		glm::vec3 Position = glm::vec3(0.f),
		glm::vec3 Origin = glm::vec3(0.f),
		glm::vec3 Rotation = glm::vec3(0.f),
		glm::vec3 Scale = glm::vec3(1.f))
	{
		this->Position = Position;
		this->Origin = Origin;
		this->Rotation = Rotation;
		this->Scale = Scale;

		this->position_location = position;
		this->color_location = color;
		this->tex_coord = tex_coords;
		this->normal_location = normal;

		this->nbrOfVertices = nbrOfVertices;
		this->nbrOfIndices = nbrOfindices;
		this->vertexArray = new Vertex[this->nbrOfVertices];
		for (size_t i = 0; i < this->nbrOfVertices; i++)
		{
			this->vertexArray[i] = vertexArray[i];
		}

		this->indexArray = new GLuint[this->nbrOfIndices];
		for (size_t i = 0; i < this->nbrOfIndices; i++)
		{
			this->indexArray[i] = indexArray[i];
		}

		this->initVAO();
		this->updateModelMatrix();
	}

	Mesh(Primitive* primitive,
		int position,
		int color,
		int tex_coords,
		int normal,
		glm::vec3 Position = glm::vec3(0.f),
		glm::vec3 Origin = glm::vec3(0.f),
		glm::vec3 Rotation = glm::vec3(0.f),
		glm::vec3 Scale = glm::vec3(1.f))
	{
		this->Position = Position;
		this->Origin = Origin;
		this->Rotation = Rotation;
		this->Scale = Scale;

		this->position_location = position;
		this->color_location = color;
		this->tex_coord = tex_coords;
		this->normal_location = normal;

		this->nbrOfVertices = primitive->getNbrOfVertices();
		this->nbrOfIndices = primitive->getNbrOfIndices();
		this->vertexArray = new Vertex[this->nbrOfVertices];
		for (size_t i = 0; i < this->nbrOfVertices; i++)
		{
			this->vertexArray[i] = primitive->getVertices()[i];
		}

		this->indexArray = new GLuint[this->nbrOfIndices];
		for (size_t i = 0; i < this->nbrOfIndices; i++)
		{
			this->indexArray[i] = primitive->getIndices()[i];
		}

		this->initVAO();
		this->updateModelMatrix();
	}

	Mesh(const Mesh& obj)
	{
		this->Position = obj.Position;
		this->Origin = obj.Origin;
		this->Rotation = obj.Rotation;
		this->Scale = obj.Scale;

		this->position_location = obj.position_location;
		this->color_location = obj.color_location;
		this->tex_coord = obj.tex_coord;
		this->normal_location = obj.normal_location;

		this->nbrOfVertices = obj.nbrOfVertices;
		this->nbrOfIndices = obj.nbrOfIndices;
		this->vertexArray = new Vertex[this->nbrOfVertices];
		for (size_t i = 0; i < this->nbrOfVertices; i++)
		{
			this->vertexArray[i] = obj.vertexArray[i];
		}

		this->indexArray = new GLuint[this->nbrOfIndices];
		for (size_t i = 0; i < this->nbrOfIndices; i++)
		{
			this->indexArray[i] = obj.indexArray[i];
		}

		this->initVAO();
		this->updateModelMatrix();
	}

	Mesh()
	{

	}

	~Mesh()
	{
		glDeleteBuffers(1, &this->VBO);
		glDeleteVertexArrays(1, &this->VAO);
		if (this->nbrOfIndices > 0)
		{
			glDeleteBuffers(1, &this->IBO);
		}

		//delete[] this->vertexArray;
		//delete[] this->indexArray;
	}

	// Accessors



	// Modifiers
	void setPosition(const glm::vec3 position)
	{
		this->Position = position;
	}

	void setOrigin(const glm::vec3 Origin)
	{
		this->Origin = Origin;
	}

	void setRotation(const glm::vec3 rotation)
	{
		this->Rotation = rotation;
	}

	void setScale(const glm::vec3 scale)
	{
		this->Scale = scale;
	}

	// Functions

	void move(const glm::vec3 position)
	{
		this->Position += position;
	}

	void rotate(const glm::vec3 rotation)
	{
		this->Rotation += rotation;
	}

	void scale(const glm::vec3 scale)
	{
		this->Scale += scale;
	}

	void update()
	{

	}

	void render(GLuint& program)
	{
		// update uniforms
		this->updateModelMatrix();
		this->updateUniforms(program);

		glUseProgram(program);

		// Bind VAO
		glBindVertexArray(this->VAO);

		// Bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		
		// Bind IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);

		// render
		if (this->nbrOfIndices == 0)
		{
			glDrawArrays(GL_TRIANGLES, 0, this->nbrOfVertices);
		}
		else
		{
			glDrawElements(GL_TRIANGLES, this->nbrOfIndices, GL_UNSIGNED_INT, 0);
		}

		//Cleanup
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glUseProgram(0);
	}
};
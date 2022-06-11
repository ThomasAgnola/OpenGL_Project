#pragma once
#include "libs.h"
#include "GLShader.h"
#include <vector>
#include "Primitives.h"

class Mesh
{
private:
	unsigned nbrOfVertices;
	unsigned nbrOfIndices;

	GLuint VAO;
	GLuint VBO;
	GLuint IBO;

	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;
	glm::mat4 ModelMatrix;

	void initVAO(Primitive* primitive,
		int position_location,
		int color_location,
		int tex_coord,
		int normal_location)
	{
		//Set variables
		this->nbrOfVertices = primitive->getNbrOfVertices();
		this->nbrOfIndices = primitive->getNbrOfIndices();

		// VAO, VBO, IBO
		// GEN VAO and BIND
		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		// GEN VBO and BIND/SEND DATA
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->nbrOfVertices * sizeof(Vertex), primitive->getVertices(), GL_STATIC_DRAW);

		// GEN IBO and BIND/SEND DATA
		if (this->nbrOfIndices > 0)
		{
			glGenBuffers(1, &this->IBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nbrOfIndices * sizeof(GLuint), primitive->getIndices(), GL_STATIC_DRAW);
		}

		const uint32_t stride = sizeof(Vertex);
		glEnableVertexAttribArray(position_location);
		glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, position));

		glEnableVertexAttribArray(color_location);
		glVertexAttribPointer(color_location, 4, GL_UNSIGNED_BYTE, /*normalisation entre [0;1]*/true, stride, reinterpret_cast<void*>(offsetof(Vertex, color)));

		glEnableVertexAttribArray(tex_coord);
		glVertexAttribPointer(tex_coord, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, texcoords));

		glEnableVertexAttribArray(normal_location);
		glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, normal));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void initVAO(Vertex* vertexArray, 
		const unsigned& nbrOfVertices, 
		GLuint* indexArray, 
		const unsigned& nbrOfindices,
		int position_location,
		int color_location,
		int tex_coord,
		int normal_location)
	{
		//Set variables
		this->nbrOfVertices = nbrOfVertices;
		this->nbrOfIndices = nbrOfindices;

		// VAO, VBO, IBO
		// GEN VAO and BIND
		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		// GEN VBO and BIND/SEND DATA
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->nbrOfVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW);

		// GEN IBO and BIND/SEND DATA
		if (this->nbrOfIndices > 0)
		{
			glGenBuffers(1, &this->IBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nbrOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW);
		}

		const uint32_t stride = sizeof(Vertex);
		glEnableVertexAttribArray(position_location);
		glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, position));
		
		glEnableVertexAttribArray(color_location);
		glVertexAttribPointer(color_location, 4, GL_UNSIGNED_BYTE, /*normalisation entre [0;1]*/true, stride, reinterpret_cast<void*>(offsetof(Vertex, color)));
		
		glEnableVertexAttribArray(tex_coord);
		glVertexAttribPointer(tex_coord, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, texcoords));
		
		glEnableVertexAttribArray(normal_location);
		glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, normal));
	
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}


	void updateUniforms(GLuint* program)
	{
		glUniformMatrix4fv(glGetUniformLocation(*program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(this->ModelMatrix));
	}

	void updateModelMatrix()
	{
		this->ModelMatrix = glm::mat4(1.f);
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->Position);
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->Rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->Rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->Rotation.z), glm::vec3(0.f, 0.f, 1.f));
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
		int normal)
	{
		this->initVAO(vertexArray, nbrOfVertices, indexArray, nbrOfindices, position, color, tex_coords, normal);
		this->updateModelMatrix();
	}

	Mesh(Primitive* primitive,
		int position,
		int color,
		int tex_coords,
		int normal)
	{
		this->initVAO(primitive, position, color, tex_coords, normal);
		this->updateModelMatrix();
	}

	Mesh()
	{

	}

	void loadMesh(
		Vertex* vertexArray,
		const unsigned& nbrOfVertices, 
		GLuint* indexArray, 
		const unsigned& nbrOfindices, 
		int position,
		int color,
		int tex_coords,
		int normal,
		glm::vec3 Position = glm::vec3(0.f),
		glm::vec3 Rotation = glm::vec3(0.f),
		glm::vec3 Scale = glm::vec3(1.f)
		)
	{
		this->Position = Position;
		this->Rotation = Rotation;
		this->Scale = Scale;
		this->initVAO(vertexArray, nbrOfVertices, indexArray, nbrOfindices, position, color, tex_coords, normal);
		this->updateModelMatrix();
	}

	void loadMesh(
		Primitive* primitive,
		int position,
		int color,
		int tex_coords,
		int normal,
		glm::vec3 Position = glm::vec3(0.f),
		glm::vec3 Rotation = glm::vec3(0.f),
		glm::vec3 Scale = glm::vec3(1.f)
		)
	{
		this->Position = Position;
		this->Rotation = Rotation;
		this->Scale = Scale;
		this->initVAO(primitive->getVertices(), primitive->getNbrOfVertices(), primitive->getIndices(), primitive->getNbrOfIndices(), position, color, tex_coords, normal);
		this->updateModelMatrix();
	}

	~Mesh()
	{
		glDeleteBuffers(1, &this->VBO);
		glDeleteVertexArrays(1, &this->VAO);
		if (this->nbrOfIndices > 0)
		{
			glDeleteBuffers(1, &this->IBO);
		}
	}

	// Accessors



	// Modifiers
	void setPosition(const glm::vec3 position)
	{
		this->Position = position;
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
		this->updateUniforms(&program);

		glUseProgram(program);

		// Bind VAO
		glBindVertexArray(this->VAO);

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
	}
};
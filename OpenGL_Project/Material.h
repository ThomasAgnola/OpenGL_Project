#pragma once
#include "libs.h"
#include "GLShader.h"

class Material
{
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLint diffuseTexture;
	GLint specularTexture;

	void setVec3f(GLuint& program, glm::fvec3 value, const GLchar* name)
	{
		glUniform3fv(glGetUniformLocation(program, name), 1, glm::value_ptr(value));
	}

	void setVec2f(GLuint& program, glm::fvec2 value, const GLchar* name)
	{
		glUniform2fv(glGetUniformLocation(program, name), 1, glm::value_ptr(value));
	}

	void setVec1f(GLuint& program, GLfloat value, const GLchar* name)
	{
		glUniform1f(glGetUniformLocation(program, name), value);
	}

	void set1li(GLuint& program, GLint value, const GLchar* name)
	{
		glUniform1i(glGetUniformLocation(program, name), value);
	}

public:
	Material()
	{

	}

	Material(
	glm::vec3 ambient,
	glm::vec3 diffuse,
	glm::vec3 specular,
	GLint diffuseTexture,
	GLint specularTexture)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->diffuseTexture = diffuseTexture;
		this->specularTexture = specularTexture;
	}

	void loadMaterial(
		glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular,
		GLint diffuseTexture,
		GLint specularTexture)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->diffuseTexture = diffuseTexture;
		this->specularTexture = specularTexture;
	}

	~Material()
	{

	}

	// Fonction
	void sendToShader(GLuint& programID)
	{
		setVec3f(programID, this->ambient, "material.ambient");
		setVec3f(programID, this->diffuse, "material.diffuse");
		setVec3f(programID, this->specular, "material.specular");
		set1li(programID, this->diffuseTexture, "material.diffuseTexture");
		set1li(programID, this->specularTexture, "material.specularTexture");
	}
};
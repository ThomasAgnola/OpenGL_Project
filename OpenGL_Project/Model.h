#pragma once
#include "libs.h"

class Model
{
private:
	Material* material;
	Texture* overrideTextureDiffuse;
	Texture* overrideTextureSpecular;
	std::vector<Mesh*> meshes;
	glm::vec3 position;

	void updateUniforms()
	{

	}


public:
	Model() {

	}

	Model(glm::vec3 position, Material* material, Texture* overrideTexDiff, Texture* overrideTexSpec, std::vector<Mesh*> meshes)
	{
		this->position = position;
		this->material = material;
		this->overrideTextureDiffuse = overrideTexDiff;
		this->overrideTextureSpecular = overrideTexSpec;
		
		for (auto*i : meshes)
		{
			this->meshes.push_back(new Mesh(*i));
		}

		for (auto& i : this->meshes)
		{
			i->move(this->position);
		}
	}

	~Model()
	{
		for (auto*& i : this->meshes)
		{
			delete i;
		}
	}

	void LoadModel(glm::vec3 position, Material* material, Texture* overrideTexDiff, Texture* overrideTexSpec, std::vector<Mesh*> meshes)
	{
		this->position = position;
		this->material = material;
		this->overrideTextureDiffuse = overrideTexDiff;
		this->overrideTextureSpecular = overrideTexSpec;

		for (auto* i : meshes)
		{
			this->meshes.push_back(new Mesh(*i));
		}
	}

	// Functions
	void rotate(const glm::vec3 rotation)
	{
		for (auto& i : this->meshes)
			i->rotate(rotation);
	}


	void update()
	{

	}

	void render(GLuint& program)
	{
		this->updateUniforms();

		// Activating Texture

		this->material->sendToShader(program);

		this->overrideTextureDiffuse->bind();

		this->overrideTextureSpecular->bind();

		for (auto& i : this->meshes)
			i->render(program);

	}

};
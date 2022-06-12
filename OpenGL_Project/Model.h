#pragma once
#include "libs.h"
#include "OBJLoader.h"

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
			i->setOrigin(this->position);
		}
	}

	Model(glm::vec3 Position, glm::vec3 scale, Material* material, Texture* overrideTexDiff, Texture* overrideTexSpec, const char* obj_file,
		int position,
		int color,
		int tex_coords,
		int normal)
	{
		this->position = Position;
		this->material = material;
		this->overrideTextureDiffuse = overrideTexDiff;
		this->overrideTextureSpecular = overrideTexSpec;

		std::vector<Vertex> mesh = loadOBJ(obj_file);
		this->meshes.push_back(new Mesh(mesh.data(), mesh.size(), NULL, 0,
			position, color, tex_coords, normal,
			glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(0.f), scale));

		for (auto& i : this->meshes)
		{
			i->move(this->position);
			i->setOrigin(this->position);
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

		for (auto& i : this->meshes)
		{
			this->overrideTextureDiffuse->bind();
			this->overrideTextureSpecular->bind();

			i->render(program);
		}

	}

};
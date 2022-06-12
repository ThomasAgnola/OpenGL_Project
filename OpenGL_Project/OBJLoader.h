#pragma once
#include "libs.h"

static std::vector<Vertex> loadOBJ(const char* filename)
{
	// Vertex parts
	std::vector<glm::fvec3> vertexPositions;
	std::vector<glm::fvec2> vertexTexcoords;
	std::vector<glm::fvec3> vertexNormals;

	// Face parts
	std::vector<GLint> vertexPositionIndices;
	std::vector<GLint> vertexTexcoordIndices;
	std::vector<GLint> vertexNormalIndices;

	// Vertex array
	std::vector<Vertex> vertices;

	std::stringstream ss;
	std::ifstream inFile(filename);
	std::string line = "";
	std::string prefixe = "";
	glm::vec3 temp_vec3;
	glm::vec2 temp_vec2;
	GLint temp_glint = 0;


	// test open file
	if (!inFile.is_open())
	{
		throw "Error couldn't open file ";
	}

	// Read each line
	while (std::getline(inFile, line))
	{
		ss.clear();
		ss.str(line);
		ss >> prefixe;

		if (prefixe == "#")
		{

		}
		else if (prefixe == "o")
		{

		}
		else if (prefixe == "s")
		{

		}
		else if (prefixe == "use_mtl")
		{

		}
		else if (prefixe == "v") // Vertex position
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertexPositions.push_back(temp_vec3);
		}
		else if (prefixe == "vt")
		{
			ss >> temp_vec2.x >> temp_vec2.y;
			vertexTexcoords.push_back(temp_vec2);
		}
		else if (prefixe == "vn") // Vertex position
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertexNormals.push_back(temp_vec3);
		}
		else if (prefixe == "f")
		{
			int counter = 0;
			while (ss >> temp_glint)
			{
				// setting indices into arrays
				if (counter == 0)
					vertexPositionIndices.push_back(temp_glint);
				else if (counter == 1)
					vertexTexcoordIndices.push_back(temp_glint);
				else if (counter == 2)
					vertexNormalIndices.push_back(temp_glint);

				// handle characters non-necessary
				if (ss.peek() == '/')
				{
					++counter;
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ')
				{
					++counter;
					ss.ignore(1, ' ');
				}

				if (counter > 2)
				{
					counter = 0;
				}
			}
		}
		else
		{

		}

		// Build final arrays
		vertices.resize(vertexPositionIndices.size(), Vertex());

		// Load all indices
		Color color = { 255, 255, 255, 255 };
		for (size_t i = 0; i < vertices.size(); i++)
		{
			vertices[i].position = vertexPositions[vertexPositionIndices[i] - 1];
			vertices[i].texcoords = vertexTexcoords[vertexTexcoordIndices[i] - 1];
			vertices[i].normal = vertexNormals[vertexNormalIndices[i] - 1];
			vertices[i].color = color;
		}

		// Debug
		//std::cout << "nbr of vertices : " << vertices.size() << "\n";
	}
	std::cout << "Obj file loaded " << "\n";
	// loaded success
	return vertices;

}
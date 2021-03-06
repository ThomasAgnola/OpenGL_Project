#pragma once
#include "libs.h"
#include "tiny_obj_loader.h"

static std::vector<Vertex> loadOBJfromlib(const char* filename)
{
	// Vertex parts
	std::vector<glm::fvec3> vertexPositions;
	std::vector<glm::fvec2> vertexTexcoords;
	std::vector<glm::fvec3> vertexNormals;

	// Vertex array
	std::vector<Vertex> vertices;
	Material material;

	glm::vec3 temp_vec3;
	glm::vec3 temp_vec3_2;
	glm::vec2 temp_vec2;

	tinyobj::ObjReader reader;
	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = "./material/";

	if (!reader.ParseFromFile(filename, reader_config))
	{
		if (!reader.Error().empty()) {
			std::cerr << "TinyObjReader: " << reader.Error();
		}
		exit(1);
	}

	if (!reader.Warning().empty()) {
		std::cout << "TinyObjReader: " << reader.Warning();
	}

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	auto& materials = reader.GetMaterials();

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				temp_vec3.x = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
				temp_vec3.y = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
				temp_vec3.z = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
				vertexPositions.push_back(temp_vec3);

				// Check if `normal_index` is zero or positive. negative = no normal data
				if (idx.normal_index >= 0) {
					temp_vec3_2.x = attrib.normals[3 * size_t(idx.normal_index) + 0];
					temp_vec3_2.y = attrib.normals[3 * size_t(idx.normal_index) + 1];
					temp_vec3_2.z = attrib.normals[3 * size_t(idx.normal_index) + 2];
					vertexNormals.push_back(temp_vec3);
				}

				// Check if `texcoord_index` is zero or positive. negative = no texcoord data
				if (idx.texcoord_index >= 0) {
					temp_vec2.x = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
					temp_vec2.y = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
					vertexTexcoords.push_back(temp_vec2);
				}

				// Optional: vertex colors
				// tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
				// tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
				// tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];

				Color color = { 255, 255, 255, 255 };
				Vertex vertex = { temp_vec3, color, temp_vec2, temp_vec3_2 };
				vertices.push_back(vertex);
			}
			index_offset += fv;

			// per-face material
			//std::cout << "Material : " << shapes[s].mesh.material_ids[f] << "\n";
		}
	}
	return vertices;

}
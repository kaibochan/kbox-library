#pragma once

#ifndef __gl_h_
#include <glad/gl.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#include <vector>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture {
	unsigned int id;
};

class Mesh {
private:
	unsigned int VAO, VBO, EBO;

	void setupMesh();

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh();
	~Mesh();

	Mesh(const std::vector<Vertex> &vertices,
		const std::vector<unsigned int> &indices, const std::vector<Texture> &textures);

	void render(Shader &shader) const;
};
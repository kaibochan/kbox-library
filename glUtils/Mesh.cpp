#include "Mesh.h"

#ifdef _DEBUG
#include <iostream>
#endif //_DEBUG

Mesh::Mesh() :
	VAO(0), VBO(0), EBO(0) {}

Mesh::Mesh(const std::vector<Vertex> &vertices,
	const std::vector<unsigned int> &indices, const std::vector<Texture> &textures) {

	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();

#ifdef _DEBUG
	//std::cout << "Allocating Mesh : [" << this << "]" << std::endl
	//	<< "VAO, VBO, EBO : [" << VAO << "], [" << VBO << "], [" << EBO << "]" << std::endl;
#endif // _DEBUG

}

void Mesh::setupMesh() {
	// Generate buffer objects
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Begin Vertex Array Object
	glBindVertexArray(VAO);
	
	// Bind vertex data to object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,
		vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// Bind semantic data to object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// Vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::normal));

	// Vertex texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::texCoords));

	// End Vertex Array Object
	glBindVertexArray(0);
}

void Mesh::render(Shader& shader) const {
	shader.use();

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Mesh::~Mesh() {
#ifdef _DEBUG
	//std::cout << "Deallocating Mesh : [" << this << "]" << std::endl
	//	<< "VAO, VBO, EBO : [" << VAO << "], [" << VBO << "], [" << EBO << "]" << std::endl;
#endif //_DEBUG

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
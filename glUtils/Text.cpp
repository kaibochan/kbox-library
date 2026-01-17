#include "Text.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Text::Text(BitmapFont* font, unsigned int VAO, unsigned int VBO, unsigned int EBO,
	unsigned int num_indices, unsigned int iVBO, unsigned int num_instances) {

	this->font = font;
	this->VAO = VAO;
	this->VBO = VBO;
	this->EBO = EBO;
	this->num_indices = num_indices;
	this->iVBO = iVBO;
	this->num_instances = num_instances;

	scale = glm::vec2(1.f, 1.f);
}

Text::~Text() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

Text* Text::generateTextMesh(BitmapFont* font, unsigned int num_instances) {
	unsigned int num_quads = 1;

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	float aspect = font->getAspectRatio();
	float w = aspect;
	float h = 1;
	vertices.insert(vertices.end(), {
		0.f,	0.f,	0.f, 0.f,
		w,		0.f,	1.f, 0.f,
		w,		h,		1.f, 1.f,
		0.f,	h,		0.f, 1.f,
		});

	indices.insert(indices.end(), {
		0, 1, 2,
		0, 2, 3,
		});

	unsigned int VBO, EBO, iVBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &iVBO);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Instance buffering
	glBindBuffer(GL_ARRAY_BUFFER, iVBO);
	glBufferData(GL_ARRAY_BUFFER, 100 * 4 * sizeof(float), NULL, GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// 

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	Text* gl_text = new Text(font, VAO, VBO, EBO, indices.size(), iVBO, num_instances);
	return gl_text;
}

void Text::updateInstances(Shader* shader) {
	instance_data.clear();

	glm::vec2 position_offset = glm::vec2(0.f);
	glm::vec2 texture_offset = glm::vec2(0.f);
	for (unsigned int i = 0; i < num_instances; i++) {
		position_offset.x = i * font->getAspectRatio();
		texture_offset = font->getCharOffset(text[i]);
		instance_data.insert(instance_data.end(), {
			// Position offset
				// Texture offset
			position_offset.x, position_offset.y,
				texture_offset.s, texture_offset.t,
			});
	}
	glBindBuffer(GL_ARRAY_BUFFER, iVBO);
	glBufferData(GL_ARRAY_BUFFER,
		instance_data.size() * sizeof(float), &instance_data[0],
		GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Text::setText(Shader* shader, std::string text) {
	if (text.length() > num_instances)
		this->text.assign(text, 0, num_instances);
	else {
		this->text = text;
		this->text += std::string(num_instances - text.length(), ' ');
	}
	updateInstances(shader);
}

std::string Text::getText() {
	return this->text;
}

void Text::render(Shader* text_shader) const {
	text_shader->use();

	glm::mat4 transform = glm::mat4(1.f);
	transform = glm::translate(transform, glm::vec3(position, 0.f));
	transform = glm::scale(transform, glm::vec3(scale, 0.f));
	glUniformMatrix4fv(text_shader->uniformLoc["transform"], 1, GL_FALSE,
		glm::value_ptr(transform));

	glUniformMatrix3fv(text_shader->uniformLoc["font_transform"], 1, GL_FALSE,
		glm::value_ptr(font->getFontTransform()));

	glBindTexture(GL_TEXTURE_2D, font->getTextureID());
	glBindVertexArray(VAO);

	glDrawElementsInstanced(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0, num_instances);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
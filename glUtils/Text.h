#pragma once

#include "Render.h"
#include "Font.h"

class Text : public Render {
private:
	BitmapFont* font;

	std::string text;

	unsigned int num_instances;
	std::vector<float> instance_data;

	unsigned int VAO, VBO, EBO;
	unsigned int iVBO;
	unsigned int num_indices;

	glm::mat4 projection;

	Text(BitmapFont* font, unsigned int VAO, unsigned int VBO, unsigned int EBO,
		unsigned int num_indices, unsigned int iVBO, unsigned int num_instances);

	void updateInstances(Shader* shader);

public:
	~Text();

	glm::vec2 position;
	glm::vec2 scale;

	void setText(Shader* shader, std::string text);
	std::string getText();

	static Text* generateTextMesh(BitmapFont* font, unsigned int length);

	void render(Shader* text_shader) const override;
};
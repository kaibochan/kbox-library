#include "Model.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

Model::Model() {

}

void Model::render(Shader *shader) const {
	shader->use();

	auto model = glm::value_ptr(transform.getTransform());
	glUniformMatrix4fv(shader->uniformLoc["model"], 1, GL_FALSE, model);

	for (const auto &mesh : meshes) {
		mesh->render(shader);
	}
}

Model::~Model() {
	for (int i = 0; i < meshes.size(); i++) {
		delete meshes[i];
	}
}
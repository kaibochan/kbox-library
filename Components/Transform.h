#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Transform {
protected:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

public:
	Transform();

	void setPosition(glm::vec3 position);
	glm::vec3 getPosition() const;

	void setRotation(glm::vec3 rotation);
	glm::vec3 getRotation() const;
	
	void setScale(glm::vec3 scale);
	glm::vec3 getScale() const;


	glm::mat4 getTransform() const;
};
#include "Transform.h"

Transform::Transform()
	: position({ 0.0f, 0.0f, 0.0f }),
	rotation({0.0f, 0.0f, 0.0f}),
	scale({1.0f, 1.0f, 1.0f}) {}

void Transform::setPosition(glm::vec3 position) {
	this->position = position;
}

void Transform::setRotation(glm::vec3 rotation) {
	this->rotation = rotation;
}

void Transform::setScale(glm::vec3 scale) {
	this->scale = scale;
}

glm::vec3 Transform::getPosition() const {
	return position;
}

glm::vec3 Transform::getRotation() const {
	return rotation;
}

glm::vec3 Transform::getScale() const {
	return scale;
}

glm::mat4 Transform::getTransform() const {
	// Translation from 0-vector to position
	glm::mat4 mat_translation = glm::translate(glm::mat4(1.0f), position);

	// Euler angle rotation (Tait-Bryan angles, x-y-z)
	glm::mat4 pitch	= glm::rotate(glm::mat4(1.0f), rotation.x, { 1.0f, 0.0f, 0.0f });
	glm::mat4 yaw	= glm::rotate(glm::mat4(1.0f), rotation.y, { 0.0f, 1.0f, 0.0f });
	glm::mat4 roll	= glm::rotate(glm::mat4(1.0f), rotation.z, { 0.0f, 0.0f, 1.0f });
	glm::mat4 mat_rotation = pitch * yaw * roll;

	// Scaling eigenvalue in each basis direction
	glm::mat4 mat_scale = glm::scale(glm::mat4(1.0f), scale);

	// S * R * T transform composition
	glm::mat4 transform = mat_translation * mat_rotation * mat_scale;
	return transform;
}
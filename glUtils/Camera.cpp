#include "Camera.h"

Camera::Camera()
	: viewZ(0.0f, 0.0f, -1.0f), viewY(0.0f, 1.0f, 0.0f),
	nearClip(0.01f), farClip(100.0f), position(0.0f, 0.0f, 0.0f),
	aspect(1.0f), fov(glm::radians(30.0f)) {}

Camera::Camera(float aspect, float fov) : Camera() {
	this->aspect = aspect;
	this->fov = fov;
}

void Camera::setViewTarget(glm::vec3 target) {
	viewZ = glm::normalize(target - position);
}

glm::mat4 Camera::getView() {
	return glm::lookAt(position, position + viewZ, viewY);
}

glm::mat4 Camera::getProjection() {
	return glm::perspective(fov, aspect, nearClip, farClip);
}

void Camera::render(Shader &shader) {
	shader.use();

	glUniformMatrix4fv(shader.uniformLoc["projection"], 1, GL_FALSE,
		glm::value_ptr(getProjection()));

	glUniformMatrix4fv(shader.uniformLoc["view"], 1, GL_FALSE,
		glm::value_ptr(getView()));
}
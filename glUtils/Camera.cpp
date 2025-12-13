#include "Camera.h"

Camera::Camera()
	: topClip(0.5f), rightClip(0.5f), nearClip(0.01f), farClip(100.0f),
	viewZ(0.0f, 0.0f, -1.0f), viewY(0.0f, 1.0f, 0.0f), position(0.0f, 0.0f, 0.0f),
	aspect(1.0f) {}

glm::mat4 Camera::getView() {
	return glm::lookAt(position, position + viewZ, viewY);
}

void Camera::setViewTarget(glm::vec3 target) {
	viewZ = glm::normalize(target - position);
}

glm::mat4 Camera::getProjection() {
	return glm::ortho(-rightClip, rightClip, -topClip, topClip, nearClip, farClip);
}

void Camera::render(Shader& shader) {
	shader.use();

	glUniformMatrix4fv(shader.uniformLoc["projection"], 1, GL_FALSE,
		glm::value_ptr(getProjection()));

	glUniformMatrix4fv(shader.uniformLoc["view"], 1, GL_FALSE,
		glm::value_ptr(getView()));
}

void Camera::render(Shader& shader, glm::mat4 man_projection) {
	shader.use();

	glUniformMatrix4fv(shader.uniformLoc["projection"], 1, GL_FALSE,
		glm::value_ptr(man_projection));

	glUniformMatrix4fv(shader.uniformLoc["view"], 1, GL_FALSE,
		glm::value_ptr(getView()));
}

PerspectiveCamera::PerspectiveCamera()
	: Camera(), fov(glm::radians(30.0f)) {}

PerspectiveCamera::PerspectiveCamera(float aspect, float fov) : PerspectiveCamera() {
	this->aspect = aspect;
	this->fov = fov;
}

glm::mat4 PerspectiveCamera::getProjection() {
	return glm::perspective(fov, aspect, nearClip, farClip);
}

DollyCamera::DollyCamera() : PerspectiveCamera() {}
DollyCamera::DollyCamera(float aspect, float fov) : PerspectiveCamera(aspect, fov) {}

glm::mat4 DollyCamera::getProjection() {
	float b, t;
	float l, r;
	float n, f;

	t = topClip;
	b = -t;
	r = t * aspect;
	l = -r;
	n = -nearClip;
	f = -farClip;

	glm::mat4 projection = glm::mat4(0.f);
	projection[0][0] = (2 * t) / (r - l);
	projection[1][1] = (2 * t) / (t - b);

	projection[2][2] = (2 * t) / (f - n) + tan(fov);
	projection[3][2] = -(2 * t * n) / (f - n) - t - n * tan(fov);

	projection[2][3] = -tan(fov);
	projection[3][3] = t - n * tan(fov);

	return projection;
}

float DollyCamera::getDollyDistance(float minDist) {
	return ((1.f) / (tan(fov) + 1.f)) * minDist;
}
#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Shader.h"

// TODO: implement optional orthogonal mode
// TODO: implement framebuffer_size_callback to fix aspect ratio

class Camera {
private:
	glm::vec3 viewZ;
	glm::vec3 viewY;

public:
	glm::vec3 position;

	float nearClip;
	float farClip;

	float aspect;
	float fov;

	Camera();
	Camera(float aspect, float fov);

	void setViewTarget(glm::vec3 target);

	glm::mat4 getView();
	glm::mat4 getProjection();

	// Handles setting appropriate uniforms for camera
	void render(Shader &shader);
};
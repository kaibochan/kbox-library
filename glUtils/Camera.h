#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Shader.h"

// TODO: implement optional orthogonal mode
//		bang head against matrix math until you can get
//		a projection function that can smoothly transition between perspective and orthogonal
//		that would be the fuckin dream man, keep dreaming
// TODO: implement framebuffer_size_callback to fix aspect ratio

class Camera {
protected:
	glm::vec3 viewZ;
	glm::vec3 viewY;

public:
	glm::vec3 position;

	float topClip;
	float rightClip;
	float nearClip;
	float farClip;
	float aspect;

	Camera();

	glm::mat4 getView();
	void setViewTarget(glm::vec3 target);

	virtual glm::mat4 getProjection();

	// Handles setting appropriate uniforms for camera
	void render(Shader& shader);
	void render(Shader& shader, glm::mat4 man_projection);
};

class PerspectiveCamera : public Camera {
public:
	float fov;

	PerspectiveCamera();
	PerspectiveCamera(float aspect, float fov);

	glm::mat4 getProjection() override;
};

class DollyCamera : public PerspectiveCamera {
public:
	DollyCamera();
	DollyCamera(float aspect, float fov);

	glm::mat4 getProjection() override;
	float getDollyDistance(float minDist);
};

class UICamera : public Camera {

};
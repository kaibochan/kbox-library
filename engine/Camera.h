#pragma once

#include "kbox.h"

#include <engine/Window.h>
#include <engine/Scene.h>
#include <engine/Events.h>

#include <glUtils/Shader.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
	glm::vec3 position;
	glm::vec3 viewZ;
	glm::vec3 viewY;

	float topClip;
	float rightClip;
	float nearClip;
	float farClip;
	float aspect;

	Camera(kbox::Window* window, kbox::Scene* scene);
	~Camera();

	glm::mat4 getView();
	void setViewTarget(glm::vec3 target);

	virtual glm::mat4 getProjection();

	// handles setting appropriate uniforms for camera
	void apply(Shader* shader);
	void apply(Shader* shader, glm::mat4 man_projection);

protected:
	virtual void updateAspect(float aspect);

private:
	Events::Framebuffer_Size framebuffer_size_callback;

	static void G_framebuffer_size_callback(void* obj_handle, GLFWwindow* handle, int width, int height);
};

class PerspectiveCamera : public Camera {
public:
	float fov;

	PerspectiveCamera(kbox::Window* window, kbox::Scene* scene);
	PerspectiveCamera(kbox::Window* window, kbox::Scene* scene, float aspect, float fov);

	glm::mat4 getProjection() override;
};

class DollyCamera : public PerspectiveCamera {
public:
	DollyCamera(kbox::Window* window, kbox::Scene* scene);
	DollyCamera(kbox::Window* window, kbox::Scene* scene, float aspect, float fov);

	glm::mat4 getProjection() override;
	float getDollyDistance(float minDist);
};
#pragma once

#include "kbox.h"

#include <GLFW/glfw3.h>

// Event Callback functors
namespace Events {
	struct Hook;
	struct Scroll;
	struct CursorPos;
	struct Key;
}

struct Events::Hook {
protected:
	void* obj_handle;
	kbox::Window* window;
	kbox::Scene* scene;
	
public:
	kbox::Scene const* getScene();
};

struct Events::Scroll : Hook {
private:
	void (*callback)(void* obj_handle, 
		GLFWwindow* window_handle, double xOffset, double yOffset) = 0;

public:
	void hook(
		void* obj_handle, kbox::Window* window,
		void (*callback)(void* obj_handle, GLFWwindow* window_handle, double xOffset, double yOffset),
		kbox::Scene* scene = NULL);
	
	void unhook();

	void operator()(GLFWwindow* window_handle, double xOffset, double yOffset);
};

struct Events::CursorPos : Hook {
private:
	void (*callback)(void* obj_handle,
		GLFWwindow* window_handle, double xPos, double yPos) = 0;

public:
	void hook(
		void* obj_handle, kbox::Window* window,
		void (*callback)(void* obj_handle, GLFWwindow* window_handle, double xPos, double yPos),
		kbox::Scene* scene = NULL);

	void unhook();

	void operator()(GLFWwindow* window_handle, double xPos, double yPos);
};

struct Events::Key : Hook {
private:
	void (*callback)(void* obj_handle,
		GLFWwindow* window_handle, int key, int scancode, int action, int mods) = 0;

public:
	void hook(
		void* obj_handle, kbox::Window* window,
		void (*callback)(void* obj_handle, GLFWwindow* window_handle, int key, int scancode, int action, int mods),
		kbox::Scene* scene = NULL);

	void unhook();

	void operator()(GLFWwindow* window_handle, int key, int scancode, int action, int mods);
};

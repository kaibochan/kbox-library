#pragma once

#include "kbox.h"

#include <GLFW/glfw3.h>

// Event Callback functors
namespace Events {
	enum Type {
		HOOK,
		FRAMEBUFFER_SIZE,
		SCROLL,
		CURSORPOS,
		KEY,
		TIMER,
		NUM_TYPES
	};

	struct Hook;
	struct Framebuffer_Size;
	struct Scroll;
	struct CursorPos;
	struct Key;
	struct Timer;
}

struct Events::Hook {
protected:
	Type type;
	void* obj_handle;
	kbox::Window* window;
	kbox::Scene* scene;
	bool active_status;
	
	void hook(void* obj_handle, kbox::Window* window, kbox::Scene* scene = NULL);
	void hook(void* obj_handle, kbox::Scene* scene);

public:
	Type getType();
	kbox::Scene const* getScene();
	void unhook();

	bool active();
	void activate();
	void deactivate();
};

struct Events::Framebuffer_Size : Hook {
private:
	void (*callback)(void* obj_handle,
		GLFWwindow* window_handle, int width, int height) = 0;

public:
	Framebuffer_Size();

	void hook(
		void* obj_handle, kbox::Window* window,
		void (*callback)(void* obj_handle, GLFWwindow* window_handle, int width, int height),
		kbox::Scene* scene = NULL
	);

	void operator()(GLFWwindow* window_handle, int width, int height);
};

struct Events::Scroll : Hook {
private:
	void (*callback)(void* obj_handle, 
		GLFWwindow* window_handle, double xOffset, double yOffset) = 0;

public:
	Scroll();

	void hook(
		void* obj_handle, kbox::Window* window,
		void (*callback)(void* obj_handle, GLFWwindow* window_handle, double xOffset, double yOffset),
		kbox::Scene* scene = NULL);
	
	void operator()(GLFWwindow* window_handle, double xOffset, double yOffset);
};

struct Events::CursorPos : Hook {
private:
	void (*callback)(void* obj_handle,
		GLFWwindow* window_handle, double xPos, double yPos) = 0;

public:
	CursorPos();

	void hook(
		void* obj_handle, kbox::Window* window,
		void (*callback)(void* obj_handle, GLFWwindow* window_handle, double xPos, double yPos),
		kbox::Scene* scene = NULL);

	void operator()(GLFWwindow* window_handle, double xPos, double yPos);
};

struct Events::Key : Hook {
private:
	void (*callback)(void* obj_handle,
		GLFWwindow* window_handle, int key, int scancode, int action, int mods) = 0;

public:
	Key();

	void hook(
		void* obj_handle, kbox::Window* window,
		void (*callback)(void* obj_handle, GLFWwindow* window_handle, int key, int scancode, int action, int mods),
		kbox::Scene* scene = NULL);

	void operator()(GLFWwindow* window_handle, int key, int scancode, int action, int mods);
};

struct Events::Timer : Hook {
private:
	double time_started;

	void (*callback)(void* obj_handle,
		GLFWwindow* window_handle, double time) = 0;

	Timer();

public:
	double delta;

	Timer(double delta);
	void start();

	void hook(
		void* obj_handle, kbox::Window* window,
		void (*callback)(void* obj_handle, GLFWwindow* window_handle, double time),
		kbox::Scene* scene = NULL);

	void operator()(GLFWwindow* window_handle, double time);
};
#include "Events.h"

#include "Window.h"

using namespace Events;

/***************
* Hook (Base)
***************/
kbox::Scene const* Hook::getScene() {
	return scene;
}

void Hook::hook(void* obj_handle, kbox::Window* window, kbox::Scene* scene) {
	this->obj_handle = obj_handle;
	this->window = window;
	this->scene = scene;

	window->register_callback(this);
}

void Hook::unhook() {
	window->remove_callback(this);
}

Type Hook::getType() { return type; }

/***************
* Framebuffer_Size
***************/

Framebuffer_Size::Framebuffer_Size() { type = FRAMEBUFFER_SIZE; }

void Framebuffer_Size::hook(void* obj_handle, kbox::Window* window,
	void (*callback)(void* obj_handle, GLFWwindow* window_handle, int width, int height),
	kbox::Scene* scene) {

	this->callback = callback;
	Hook::hook(obj_handle, window, scene);
}

void Framebuffer_Size::operator()(GLFWwindow* window_handle, int width, int height) {
	(*callback)(obj_handle, window_handle, width, height);
}

/***************
* Scroll
***************/
Scroll::Scroll() { type = SCROLL; }

void Scroll::hook(
	void* obj_handle, kbox::Window* window,
	void (*callback)(void* obj_handle, GLFWwindow* window_handle, double xOffset, double yOffset),
	kbox::Scene* scene) {

	this->callback = callback;
	Hook::hook(obj_handle, window, scene);
}

void Scroll::operator()(GLFWwindow* window_handle, double xOffset, double yOffset) {
	(*callback)(obj_handle, window_handle, xOffset, yOffset);
}

/***************
* CursorPos
***************/
CursorPos::CursorPos() { type = CURSORPOS; }

void CursorPos::hook(void* obj_handle, kbox::Window* window,
	void (*callback)(void* obj_handle, GLFWwindow* window_handle, double xPos, double yPos),
	kbox::Scene* scene) {

	this->callback = callback;
	Hook::hook(obj_handle, window, scene);
}

void CursorPos::operator()(GLFWwindow* window_handle, double xPos, double yPos) {
	(*callback)(obj_handle, window_handle, xPos, yPos);
}

/***************
* Key
***************/
Key::Key() { type = KEY; }

void Key::hook(void* obj_handle, kbox::Window* window,
	void (*callback)(void* obj_handle, GLFWwindow* window_handle, int key, int scancode, int action, int mods),
	kbox::Scene* scene) {

	this->callback = callback;
	Hook::hook(obj_handle, window, scene);
}

void Key::operator()(GLFWwindow* window_handle, int key, int scancode, int action, int mods) {
	(*callback)(obj_handle, window_handle, key, scancode, action, mods);
}
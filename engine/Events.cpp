#include "Events.h"

#include "Window.h"

using namespace Events;

/***************
* Hook (Base)
***************/
kbox::Scene const* Hook::getScene() {
	return scene;
}

/***************
* Scroll
***************/
void Scroll::hook(
	void* obj_handle, kbox::Window* window,
	void (*callback)(void* obj_handle, GLFWwindow* window_handle, double xOffset, double yOffset),
	kbox::Scene* scene) {

	this->obj_handle = obj_handle;
	this->callback = callback;
	this->window = window;
	this->scene = scene;

	window->register_scroll_callback(this);
}

void Scroll::unhook() {
	window->remove_scroll_callback(this);
}

void Scroll::operator()(GLFWwindow* window_handle, double xOffset, double yOffset) {
	(*callback)(obj_handle, window_handle, xOffset, yOffset);
}

/***************
* CursorPos
***************/
void CursorPos::hook(void* obj_handle, kbox::Window* window,
	void (*callback)(void* obj_handle, GLFWwindow* window_handle, double xPos, double yPos),
	kbox::Scene* scene) {

	this->obj_handle = obj_handle;
	this->callback = callback;
	this->window = window;
	this->scene = scene;

	window->register_cursor_pos_callback(this);
}

void CursorPos::unhook() {
	window->remove_cursor_pos_callback(this);
}

void CursorPos::operator()(GLFWwindow* window_handle, double xPos, double yPos) {
	(*callback)(obj_handle, window_handle, xPos, yPos);
}

/***************
* Key
***************/
void Key::hook(void* obj_handle, kbox::Window* window,
	void (*callback)(void* obj_handle, GLFWwindow* window_handle, int key, int scancode, int action, int mods),
	kbox::Scene* scene) {

	this->obj_handle = obj_handle;
	this->callback = callback;
	this->window = window;
	this->scene = scene;

	window->register_key_callback(this);
}

void Key::unhook() {
	window->remove_key_callback(this);
}

void Key::operator()(GLFWwindow* window_handle, int key, int scancode, int action, int mods) {
	(*callback)(obj_handle, window_handle, key, scancode, action, mods);
}
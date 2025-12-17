#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "kbox.h"
#include "Events.h"

#include <vector>
#include <list>
#include <map>


class kbox::Window {
public:
	Window(GLFWwindow* window_handle);
	~Window();

	void registerScene(Scene* scene);
	const std::list<Scene*> getRegisteredScenes();

	void loadScene(Scene* scene);
	void unloadScene(Scene* scene);

	void setScene(Scene* scene);
	Scene* getScene();

	void register_callback(Events::Hook* callback);
	void remove_callback(Events::Hook* callback);

	//void register_scroll_callback(Events::Scroll* callback);
	//void remove_scroll_callback(Events::Scroll* callback);

	//void register_cursor_pos_callback(Events::CursorPos* callback);
	//void remove_cursor_pos_callback(Events::CursorPos* callback);

	//void register_key_callback(Events::Key* callback);
	//void remove_key_callback(Events::Key* callback);

	GLFWwindow* getWindowHandle();

	int getWidth();
	int getHeight();
	float getAspectRatio();

private:
	Context* parent;
	GLFWwindow* window_handle;

	Scene* current_scene;
	std::list<Scene*> scenes;

	std::map<Events::Type, std::list<Events::Hook*>> callbacks;
	/*std::list<Events::Scroll*> scroll_callbacks;
	std::list<Events::CursorPos*> cursor_pos_callbacks;
	std::list<Events::Key*> key_callbacks;*/

	int width, height;

	static void G_window_close_callback(GLFWwindow* window_handle);
	static void G_framebuffer_size_callback(GLFWwindow* window_handle, int width, int height);
	static void G_scroll_callback(GLFWwindow* window_handle, double xOffset, double yOffset);
	static void G_cursor_position_callback(GLFWwindow* window_handle, double xPos, double yPos);
	static void G_key_callback(GLFWwindow* window_handle, int key, int scancode, int action, int mods);

	void setCallbacks();

	void render(double deltaTime);

	friend class Context;
	//friend class Events::Hook;
};
#include "Window.h"

#include "Engine.h"
#include "Scene.h"

#ifdef _DEBUG
#include <iostream>
#endif //_DEBUG

using namespace kbox;

void Window::setCallbacks() {
	for (int i = 1; i < Events::NUM_TYPES; i++) {
		callbacks.insert({ (Events::Type)i, std::list<Events::Hook*>() });
	}
	
	glfwSetWindowCloseCallback(window_handle, Window::G_window_close_callback);
	glfwSetFramebufferSizeCallback(window_handle, Window::G_framebuffer_size_callback);
	glfwSetScrollCallback(window_handle, Window::G_scroll_callback);
	glfwSetCursorPosCallback(window_handle, Window::G_cursor_position_callback);
	glfwSetKeyCallback(window_handle, Window::G_key_callback);
}

Window::Window(GLFWwindow* window_handle) {
	this->window_handle = window_handle;
	glfwGetWindowSize(window_handle, &width, &height);
	glfwSetWindowUserPointer(window_handle, this);
	setCallbacks();
}

Window::~Window() {
	GLFWwindow* previous_context = glfwGetCurrentContext();
	glfwMakeContextCurrent(window_handle);
	
	for (auto it = scenes.begin(); it != scenes.end();) {
		(*it)->terminate();

#ifdef _DEBUG
		std::cout << "Terminating scene [" << *it << "]" << std::endl;
#endif // _DEBUG

		delete *it;
		it = scenes.erase(it);
	}

	glfwMakeContextCurrent(previous_context);
	glfwDestroyWindow(window_handle);
}

void Window::registerScene(Scene* scene) {
	scenes.push_back(scene);
}

const std::list<Scene*> Window::getRegisteredScenes() {
	return scenes;
}

void Window::loadScene(Scene* scene) {
	bool scene_loaded = false;

	for (auto it = scenes.begin(); it != scenes.end(); it++) {
		if (scene_loaded = (*it == scene)) {
			GLFWwindow* prev_context = glfwGetCurrentContext();
			glfwMakeContextCurrent(window_handle);
			(*it)->initialize();
			glfwMakeContextCurrent(prev_context);

#ifdef _DEBUG
			std::cout << "Window [" << this << ", "
				<< glfwGetWindowTitle(window_handle) << "] loaded a scene" << std::endl;
#endif // _DEBUG

			break;
		}
	}

#ifdef _DEBUG
	if (!scene_loaded) {
		std::cerr << "ERROR::Scene has not been registered with Window [" << this << ", "
			<< glfwGetWindowTitle(window_handle) << "]" << std::endl;
	}
#endif // _DEBUG
}

void Window::unloadScene(Scene* scene) {
	bool scene_unloaded = false;

	for (auto it = scenes.begin(); it != scenes.end(); it++) {
		if (scene_unloaded = (*it == scene)) {
			(*it)->terminate();

#ifdef _DEBUG
			std::cout << "Window [" << this << ", "
				<< glfwGetWindowTitle(window_handle) << "] unloaded a scene" << std::endl;
#endif // _DEBUG

			break;
		}
	}

#ifdef _DEBUG
	if (!scene_unloaded) {
		std::cerr << "ERROR::Scene has not been registered with Window [" << this << ", "
			<< glfwGetWindowTitle(window_handle) << "]" << std::endl;
	}
#endif // _DEBUG
}


void Window::setScene(Scene* scene) {
	auto it = std::find(scenes.begin(), scenes.end(), scene);
	if (it != scenes.end()) {
		current_scene = *it;
		current_scene->resume();
		return;
	}

#ifdef _DEBUG
	std::cerr << "ERROR::Scene has not registered with window: "
		<< glfwGetWindowTitle(window_handle) << std::endl;
#endif // _DEBUG

}

Scene* Window::getScene() {
	return current_scene;
}

void Window::render(double deltaTime) {
	// Process scene
	current_scene->process(deltaTime);

	// Render scene
	current_scene->render();

	// Swap buffers and poll for next events
	glfwSwapBuffers(window_handle);
}

GLFWwindow* Window::getWindowHandle() {
	return window_handle;
}

int Window::getWidth() {
	return width;
}

int Window::getHeight() {
	return height;
}

float Window::getAspectRatio() {
	return width / (float)height;
}

void Window::register_callback(Events::Hook* callback) {
	callbacks[callback->getType()].push_back(callback);
}

void Window::remove_callback(Events::Hook* callback) {
	std::list<Events::Hook*>& group = callbacks[callback->getType()];
#ifdef _DEBUG
	std::cout << "Removed callback [" << callback << "], "
		<< "Type: " << callback->getType() << std::endl;
#endif //_DEBUG

	group.remove(callback);
}

void Window::G_window_close_callback(GLFWwindow* window_handle) {
	Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window_handle));
	if (!window)
		return;

	Engine::closeWindow(window);
}

void Window::G_framebuffer_size_callback(GLFWwindow* window_handle, int width, int height) {
	Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window_handle));
	if (!window)
		return;

	GLFWwindow* prev_context = glfwGetCurrentContext();
	glfwMakeContextCurrent(window->window_handle);

	glfwGetWindowSize(window_handle, &(window->width), &(window->height));
	glViewport(0, 0, width, height);
	glfwMakeContextCurrent(prev_context);

	for (auto callback : window->callbacks[Events::FRAMEBUFFER_SIZE]) {
		if (!callback->getScene() || callback->getScene()->getStatus() == Scene::ACTIVE)
			(*reinterpret_cast<Events::Framebuffer_Size*>(callback))(window_handle, width, height);
	}
}

void Window::G_scroll_callback(GLFWwindow* window_handle, double xOffset, double yOffset) {
	Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window_handle));
	if (!window)
		return;

	for (auto callback : window->callbacks[Events::SCROLL]) {
		if (!callback->getScene() || callback->getScene()->getStatus() == Scene::ACTIVE)
			(*reinterpret_cast<Events::Scroll*>(callback))(window_handle, xOffset, yOffset);
	}
}

void Window::G_cursor_position_callback(GLFWwindow* window_handle, double xPos, double yPos) {
	Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window_handle));
	if (!window)
		return;

	for (auto callback : window->callbacks[Events::CURSORPOS]) {
		if (!callback->getScene() || callback->getScene()->getStatus() == Scene::ACTIVE)
			(*reinterpret_cast<Events::CursorPos*>(callback))(window_handle, xPos, yPos);
	}
}

void Window::G_key_callback(GLFWwindow* window_handle, int key, int scancode, int action, int mods) {
	Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window_handle));
	if (!window)
		return;

	for (auto callback : window->callbacks[Events::KEY]) {
		if (!callback->getScene() || callback->getScene()->getStatus() == Scene::ACTIVE)
			(*reinterpret_cast<Events::Key*>(callback))(window_handle, key, scancode, action, mods);
	}
}
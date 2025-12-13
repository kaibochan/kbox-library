#include "Context.h"

#include <AL/al.h>
#include <glad/gl.h>

#include "Engine.h"
#include "Window.h"
#include "Scene.h"

#ifdef _DEBUG
#include "Memory.h"
#include <iostream>
#endif //_DEBUG

using namespace kbox;

Context::Context() {}

Context::~Context() {
	// Destroy all GLFWwindow objects and their shared context
	for (auto it = windows.begin(); it != windows.end();) {
		delete *it;
		it = windows.erase(it);
	}

	alcDestroyContext(audio);
}

void Context::openAudioDevice(const char* name = NULL) {
	alGetError();
	ALCdevice* al_device = alcOpenDevice(name);
	if (!al_device) {
		Engine::notify(AUDIO_DEVICE_FAILED);
		return;
	}

	audio = alcCreateContext(al_device, NULL);
	if (!audio) {
		Engine::notify(AUDIO_CONTEXT_FAILED);
		return;
	}
}

Window* Context::createWindow(int width, int height, const char* title,
	GLFWmonitor* monitor) {

	GLFWwindow* sharedContext = NULL;
	if (!windows.empty()) {
		sharedContext = windows.front()->getWindowHandle();

#ifdef _DEBUG
		std::cout << "Creating shared context window: " << title << std::endl
			<< "\tSharing with: " << glfwGetWindowTitle(sharedContext) << std::endl;
#endif //_DEBUG
	}

	// Set OpenGL version and profile constraints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window_handle =
		glfwCreateWindow(width, height, title, monitor, sharedContext);

	if (window_handle == NULL) {
		Engine::notify(kbox::WINDOW_CREATION_FAILED);
		return NULL;
	}

	GLFWwindow* previous_context = glfwGetCurrentContext();
	glfwMakeContextCurrent(window_handle);

	// Load OpenGL functions
	
	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
		Engine::notify(kbox::GLAD_LOADER_FAILED);
		return NULL;
	}

	glViewport(0, 0, width, height);
	glfwMakeContextCurrent(previous_context);

	Window* window = new Window(window_handle);
	window->parent = this;
	windows.push_back(window);

	return window;
}

void Context::closeWindow(Window* window) {
	glfwSetWindowShouldClose(window->window_handle, GLFW_TRUE);
	
	for (auto it = windows.begin(); it != windows.end(); it++) {
		if (*it == window) {
#ifdef _DEBUG
			std::cout << "Closing window: "
				<< glfwGetWindowTitle((*it)->window_handle) << std::endl;
#endif //_DEBUG
			delete *it;
			windows.erase(it);

			break;
		}
	}
}

// TODO: Handle event polling for multiple contexts
// e.g. re-evaluate placement of glfwPollEvents
void Context::run() {
	double deltaTime;
	double previousTime = glfwGetTime();

	if (audio)
		alcMakeContextCurrent(audio);
	else
		Engine::notify(kbox::CONTEXT_USE_FAILED_NO_AUDIO);

#ifdef _MEM
	unsigned int numCycles = 0;
	unsigned int interval = 60;
#endif // _MEM
	while (!windows.empty()) {
		deltaTime = glfwGetTime() - previousTime;
		previousTime = glfwGetTime();

		for (auto it = windows.begin(); it != windows.end(); it++) {
			glfwMakeContextCurrent((*it)->window_handle);
			(*it)->render(deltaTime);
		}
		
		glfwPollEvents();

#ifdef _MEM
		numCycles++;

		if (numCycles % interval == 0)
			kbox::Mem::inspect();
#endif // _MEM
	}
}
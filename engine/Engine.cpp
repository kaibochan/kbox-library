#include "Engine.h"

#include "Window.h"

#ifdef _DEBUG
#include "Memory.h"
#include <iostream>
#endif //_DEBUG

// Use GPU instead of integrated graphics
#ifdef WIN32
#include <Windows.h>
extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif //_WIN32

using namespace kbox;

//std::list<Context*> Engine::contexts;
//Context* Engine::current_context;
Error Engine::error_state;

std::list<Window*> Engine::windows;
ALCcontext* Engine::audio;

int Engine::initialize() {

	if (!glfwInit())
		return error_state = GLFW_INIT_FAIL;
	return error_state = NO_ERROR;
}

void Engine::notify(Error error) {
#ifdef _DEBUG
	std::cerr << "ERROR::Engine error occurred: " << error << std::endl;
#endif //_DEBUG
	error_state = error;
}

Error Engine::getError() {
	return error_state;
}

void Engine::terminate() {
	// Destroy all GLFWwindow objects and their shared context
	for (auto it = windows.begin(); it != windows.end();) {
		delete* it;
		it = windows.erase(it);
	}

	if (audio)
		alcDestroyContext(audio);

	glfwTerminate();
}



void Engine::openAudioDevice(const char* name = NULL) {
	alGetError();
	ALCdevice* al_device = alcOpenDevice(name);
	if (!al_device) {
		notify(AUDIO_DEVICE_FAILED);
		return;
	}

	audio = alcCreateContext(al_device, NULL);
	if (!audio) {
		notify(AUDIO_CONTEXT_FAILED);
		return;
	}
}


Window* Engine::createWindow(int width, int height, const char* title,
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
	windows.push_back(window);

	return window;
}


void Engine::closeWindow(Window* window) {
	glfwSetWindowShouldClose(window->window_handle, GLFW_TRUE);

#ifdef _DEBUG
	std::cout << "Closing window: "
		<< glfwGetWindowTitle(window->window_handle) << std::endl;
#endif //_DEBUG

	delete window;
	windows.remove(window);
}

void Engine::run() {
	double deltaTime;
	double previousTime = glfwGetTime();

	if (audio)
		alcMakeContextCurrent(audio);
	else
		notify(kbox::CONTEXT_USE_FAILED_NO_AUDIO);

	while (!windows.empty()) {
		deltaTime = glfwGetTime() - previousTime;
		previousTime = glfwGetTime();

		for (auto it = windows.begin(); it != windows.end(); it++) {
			glfwMakeContextCurrent((*it)->window_handle);
			(*it)->render(deltaTime);
		}

		glfwPollEvents();
	}
}
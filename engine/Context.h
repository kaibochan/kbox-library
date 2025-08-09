#pragma once

#include <GLFW/glfw3.h>
#include <AL/alc.h>

#include "kbox.h"

#include <list>

// Context encapsulates an openGL and openAL context
// A context may have multiple windows and one audio device
class kbox::Context {
public:
	Context();
	~Context();

	/**
	* Open audio device for AL audio calls.
	* @param name Name of the audio device.
	*	Use NULL to refer to the default device.
	*/
	void openAudioDevice(const char* name);
	/**
	* Create a window for GL draw calls.
	* @param width Width of created window.
	* @param height Height of created window.
	* @param title Title of created window.
	* @param monitor Which monitor to fullscreen the created window.
	*	Use NULL if created window should not be fullscreen.
	* @return Window handle for use with a scene
	*/
	Window* createWindow(int width, int height, const char* title, GLFWmonitor* monitor = NULL);
	void closeWindow(Window* window);

	// Runs all windows and their current scenes
	void run();

private:
	int error_state;

	std::list<Window*> windows;
	ALCcontext* audio;

	Context& operator=(const Context& other) = delete;
	Context(const Context& other) = delete;

	friend class Engine;
};
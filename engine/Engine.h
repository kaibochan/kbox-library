#pragma once

#include <GLFW/glfw3.h>

#include <alUtils/Device.h>
#include <alUtils/SoundManager.h>

#include "kbox.h"

#include <list>

// Static class which manages OpenGL and OpenAL contexts and their respective
// windows and devices
class kbox::Engine {
private:
	static Error error_state;
	static std::list<Context*> contexts;

	static Context* current_context;

public:
	/**
	* Initialize Engine environment.
	* @return Error state after initialization.
	*/
	static int initialize();
	
	/**
	* Create a context for use with a scene.
	* @return Newly created context.
	*/
	static Context* createContext();

	/**
	* Gets the current context the Engine is using.
	* @return Context currently in use.
	*/
	static Context* getContext();

	/**
	* Notify engine of error.
	* @param error The kbox::Error which has occured.
	*/
	static void notify(Error error);

	static kbox::Error getError();

	// Terminate the Engine and clean up all contexts
	static void terminate();
};
#include "Engine.h"

#include "Context.h"
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

std::list<Context*> Engine::contexts;
Context* Engine::current_context;
Error Engine::error_state;

int Engine::initialize() {
#ifdef _MEM
	Mem::initialize();
#endif // _MEM

	if (!glfwInit())
		return error_state = GLFW_INIT_FAIL;
	return error_state = NO_ERROR;
}

Context* Engine::createContext() {
	Context* context = new Context();
	contexts.push_back(context);
	return context;
}

Context* Engine::getContext() {
	return current_context;
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
	// Delete all contexts
	for (auto it = contexts.begin(); it != contexts.end();) {
		delete (*it);
		it = contexts.erase(it);
	}
}
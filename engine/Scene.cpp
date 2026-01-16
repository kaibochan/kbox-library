#include "Scene.h"

#include "Window.h"

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

using namespace kbox;

Scene::Scene(Window* window){
	this->window = window;
	status = UNLOADED;
}

void Scene::initialize() {
	if (status != UNLOADED)
		return;

	load();
	status = ACTIVE;

#ifdef _DEBUG
	std::cout << "Scene [" << this << "] loaded" << std::endl;
#endif // _DEBUG
}

void Scene::terminate() {
	if (status == UNLOADED)
		return;

	unload();
	status = UNLOADED;

#ifdef _DEBUG
	std::cout << "Scene [" << this << "] unloaded" << std::endl;
#endif // _DEBUG
}

void Scene::resume() {
	status = ACTIVE;
}

void Scene::suspend() {
	status = INACTIVE;
}

Scene::Status Scene::getStatus() const {
	return status;
}

Window* Scene::getWindow() const {
	return window;
}

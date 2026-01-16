#pragma once

#include "kbox.h"
#include <GLFW/glfw3.h>

#include "kbox.h"

class kbox::Scene {
public:
	enum Status {
		UNLOADED = 0x00,
		ACTIVE = 0x01,
		INACTIVE = 0x02,
	};

	Scene(Window* window);

	void resume();
	void suspend();
	Status getStatus() const;

	Window* getWindow() const;

protected:
	Window* window;
	Status status;

	virtual void process(double deltaTime) = 0;
	virtual void render() = 0;
	virtual void load() = 0;
	virtual void unload() = 0;

private:
	void initialize();
	void terminate();

	friend class Window;
};
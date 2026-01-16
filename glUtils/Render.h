#pragma once

#include "Shader.h"

class Render {
public:
	virtual void render(Shader* shader) const = 0;
};
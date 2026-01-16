#pragma once

#include <Components/Transform.h>
#include <engine/Camera.h>

#include <glUtils/Render.h>
#include <glUtils/Mesh.h>
#include <glUtils/Shader.h>

#include <alUtils/Source.h>

#include <vector>

// TODO: Implement proper destructor and cleanup of GL data

class Model : public Render {
public:
	Transform transform;

	// Multiple meshes; each are relative to model's transform
	std::vector<Mesh*> meshes;

	// Multiple audio sources
	// Each source has a transform of its own relative to the model's transform
	std::vector<Audio::Source> sources;

public:
	Model();
	~Model();
	
	virtual void render(Shader* shader) const override;
};
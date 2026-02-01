#pragma once
#include "../Utilities/utilities.h"
#include "ModelResource.h"
#include "ShaderResource.h"
#include "TextureResource.h"

class SceneObject {
public:
	GLuint id;
	float position[3];
	Vector3 rotation;
	Vector3 scale;
	ModelResource* model;
	ShaderResource* shader;
	TextureResource* texture;
	int depthTest;

	SceneObject();
	~SceneObject();
	void Draw(ESContext* esContext);
	void Update(float deltaTime);	
};


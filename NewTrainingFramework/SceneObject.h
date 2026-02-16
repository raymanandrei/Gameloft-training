#pragma once
#include "../Utilities/utilities.h"
#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

class SceneObject {
public:
	GLuint id;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	Vector3 color;	

	Model* model;
	Shader* shader;
	Texture* texture;

	Camera camera;
	Matrix MVP;

	int depthTest;

	SceneObject();
	~SceneObject();
	void sendCommonData(ESContext* esContext);
	void Draw(ESContext* esContext);
	void Update(float deltaTime);	
};


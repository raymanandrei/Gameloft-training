#pragma once

#include "ModelResource.h"

#include "../Utilities/utilities.h"

class Model
{
public:
	Model();
	~Model();

	bool Load();

	ModelResource* mr;
	GLuint iboId;
	GLuint wiredIboId;
	GLuint vboId;
	int nrIndici;
	int nrIndiciWired;
};


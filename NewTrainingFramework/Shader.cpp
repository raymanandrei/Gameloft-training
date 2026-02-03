#include "stdafx.h"
#include "Shader.h"

#include "../Utilities/utilities.h"

Shader::Shader() {
}

Shader::~Shader() {
}

bool Shader::Load() {
	GLuint vertexShader = esLoadShader(GL_VERTEX_SHADER, (char*)(sr->vs).c_str());

	if (vertexShader == 0)
		return -1;

	GLuint fragmentShader = esLoadShader(GL_FRAGMENT_SHADER,(char *)(sr->fs).c_str());

	if (fragmentShader == 0)
	{
		glDeleteShader(vertexShader);
		return -2;
	}

	programId = esLoadProgram(vertexShader, fragmentShader);

	return true;
}
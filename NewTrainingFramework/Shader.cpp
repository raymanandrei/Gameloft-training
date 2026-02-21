#include "stdafx.h"
#include "Shader.h"

#include "../Utilities/utilities.h"

Shader::Shader() {
}

Shader::~Shader() {
}

bool Shader::Load() {
	//printf("Loading shader with vertex shader: %s and fragment shader: %s\n", sr->vs.c_str(), sr->fs.c_str());
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

	sr->positionAttribute = glGetAttribLocation(programId, "a_posL");
	sr->colorAttribute = glGetAttribLocation(programId, "a_color");
	sr->objectColor = glGetUniformLocation(programId, "objectColor");
	sr->matrixCamera = glGetUniformLocation(programId, "MVP");
	for (int i = 0; i < 5; i++) {
		std::string con = "u_texture_" + std::to_string(i);
		sr->textureUniform[i] = glGetUniformLocation(programId,(char * )con.c_str());
	}
	sr->uvAttribute = glGetAttribLocation(programId, "a_uv");
	sr->uv2Attribute = glGetAttribLocation(programId, "a_uv2");

	return true;
}
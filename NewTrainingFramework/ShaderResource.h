#pragma once
#include <string>

struct ShaderResource
{
	GLuint program, vertexShader, fragmentShader;
	std::string id;
	std::string vs;
	std::string fs;
	GLint positionAttribute;
	GLint colorAttribute;
	GLint objectColor;
	GLint matrixUniform;
	GLint matrixCamera;
	GLint textureUniform[5];
	GLint uvAttribute;
	GLint uv2Attribute;
	GLint u_height;
};
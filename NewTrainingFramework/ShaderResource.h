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
	GLint matrixUniform;
	GLint matrixCamera;
	GLint textureUniform;
	GLint uvAttribute;
	GLint colorVarying;
};
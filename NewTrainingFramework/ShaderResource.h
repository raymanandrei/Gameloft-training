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
	GLint blendTextureOffset;
	GLint smallR;
	GLint bigR;
	GLint fogColor;
	GLint cameraPosition;
	GLint matrixModel;
	GLint U_Time;
	GLint U_DispMax;
};
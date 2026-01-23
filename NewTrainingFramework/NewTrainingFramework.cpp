// NewTrainingFramework.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include <conio.h>
#include "Globals.h"
#include "Camera.h"
#include "ResourceManager.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define PI 3.14159265358979323846

GLuint modelVboId;
GLuint modelIboId;
GLuint idTexture;

std::vector<Vertex> vertices;
std::vector<unsigned short> indices;

Shaders modelShader;

float angle = 0;
float step = 0.05;

float totalTime = 0;

Camera camera = Camera();

ResourceManager* resourceManager = ResourceManager::GetInstance();

void readNfgLine(std::string str,std::string field, std::vector<float> &numbers) {
	//std::cout << str << " ";
	int posIndex = str.find(field);
	//std::cout << posIndex << "\n";
	if (posIndex != -1) {

		//std::cout << "Field:" << str.find(';') << "\n";
		//std::cout <<"substring"<< str.substr(posIndex + field.length() + 1, str.find(';') - posIndex - field.length() - 1) << '\n';
		std::string numbersStr = str.substr(posIndex + field.length() + 1, str.find(';') - posIndex - field.length() - 1);
		std::stringstream ss(numbersStr);
		std::string token;

		while (std::getline(ss, token, ',')) {
			numbers.push_back(std::stod(token)); 
		}
		//for (double f : numbers) {
		//	std::cout << field << f << " ";
		//}
		//std::cout << '\n';
	}
}

void readNfg(const char* filename, std::vector<Vertex>& vertices, std::vector<unsigned short>& indices) {
	std::ifstream file(filename);
	std::string str;
	while (std::getline(file, str))
	{	
		std::vector<float> numbers;
		Vertex v;
		readNfgLine(str, "pos:", numbers);

		if (numbers.size() == 3) {
			
			v.pos.x = numbers[0];
			v.pos.y = numbers[1];
			v.pos.z = numbers[2];
		}

		str = str.substr(str.find(";") + 1,str.length() - str.find(";") - 1);
		numbers.clear();
		readNfgLine(str, "norm:", numbers);

		if (numbers.size() == 3) {
			v.norm.x = numbers[0];
			v.norm.y = numbers[1];
			v.norm.z = numbers[2];
		}

		str = str.substr(str.find(";") + 1, str.length() - str.find(";") - 1);
		numbers.clear();
		readNfgLine(str, "binorm:", numbers);

		if (numbers.size() == 3) {
			v.binorm.x = numbers[0];
			v.binorm.y = numbers[1];
			v.binorm.z = numbers[2];
		}

		str = str.substr(str.find(";") + 1, str.length() - str.find(";") - 1);

		numbers.clear();
		readNfgLine(str, "tgt:", numbers);

		//std::cout << "Numbers size norm:" << numbers.size() << "\n";
		//for (double f : numbers) {
		//	std::cout << "binorm:" << f << " ";
		//}
		//std::cout << '\n';

		if (numbers.size() == 3) {
			v.tgt.x = numbers[0];
			v.tgt.y = numbers[1];
			v.tgt.z = numbers[2];
		}

		str = str.substr(str.find(";") + 1, str.length() - str.find(";") - 1);

		numbers.clear();
		readNfgLine(str, "uv:", numbers);

		if (numbers.size() == 2) {
			v.uv.x = numbers[0];
			v.uv.y = numbers[1];
			vertices.push_back(v);
		}

		int posIndex = str.find("NrIndices:");
		if (posIndex != std::string::npos) {
			std::string numberStr = str.substr(posIndex + 10);
			int nrIndices = std::stoi(numberStr);
			std::cout << "NrIndices: " << nrIndices << "\n";
			while (std::getline(file, str)) {

				int posIndex = str.find(".");

				std::string numbersStr = str.substr(posIndex + 1, str.find('.', posIndex + 1) - posIndex - 1);
				std::stringstream ss(numbersStr);
				std::string token;

				while (std::getline(ss, token, ',')) {
					indices.push_back(std::stoi(token));
				}

			}
		}

	}
}

int Init ( ESContext *esContext )
{
	int width, height, bpp;
	char *pixelArray;

	glEnable(GL_DEPTH_TEST);
	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );

	readNfg("../../NewResourcesPacket/Models/Croco.nfg", vertices, indices);

	for (auto idx : indices) {
		std::cout << "Index: " << idx << "\n";
	}

	for (auto v : vertices) {
		std::cout << "Vertex pos (Init): " << v.uv.x << ", " << v.uv.y << ", " << v.pos.y << "\n";
	}
	
	for (auto v: vertices) {
		v.color.x = 1.0f;
		v.color.y = 1.0f;
		v.color.z = 1.0f;
	}

	glGenBuffers(1, &modelVboId);
	glGenBuffers(1, &modelIboId);
	
	glBindBuffer(GL_ARRAY_BUFFER, modelVboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelIboId);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(std::vector<unsigned short>),indices.data(),GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	resourceManager->Init();

	pixelArray = LoadTGA("C:/Users/Andrei/Desktop/Gameloft/NewResourcesPacket/Textures/Croco.tga", &width, &height, &bpp);

	std::cout << "Texture width: " << width << " height: " << height << " bpp: " << bpp << "\n";

	glGenTextures(1, &idTexture);
	glBindTexture(GL_TEXTURE_2D, idTexture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,pixelArray);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	return modelShader.Init("../Resources/Shaders/ModelShaderVS.vs", "../Resources/Shaders/ModelShaderFS.fs");

}

void Draw ( ESContext *esContext )
{
	Matrix mRotation;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(modelShader.program);

	glBindBuffer(GL_ARRAY_BUFFER, modelVboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelIboId);

	if (modelShader.positionAttribute != -1)
	{
		glEnableVertexAttribArray(modelShader.positionAttribute);
		glVertexAttribPointer(modelShader.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (modelShader.textureUniform != - 1)
	{
		glUniform1i(modelShader.textureUniform, 0);
	}

	std::cout << modelShader.uvAttribute << "\n";

	if (modelShader.uvAttribute != -1)
	{
		glEnableVertexAttribArray(modelShader.uvAttribute);
		glVertexAttribPointer(modelShader.uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, uv));
	}


	Matrix MVP = camera.viewMatrix * camera.perspectiveMatrix;

	if (modelShader.matrixCamera != -1) {
		glUniformMatrix4fv(modelShader.matrixCamera, 1, GL_FALSE, (float*)MVP.m);
	}

	glDrawElements(GL_TRIANGLES,indices.size(), GL_UNSIGNED_SHORT, 0);

	GLint err = glGetError();
	std::cout << "GL Error: " << err << "\n";

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
} 

void Update ( ESContext *esContext, float deltaTime )
{
	//printf("Update: %f\n", deltaTime);
	totalTime += deltaTime;
	if (totalTime >= Globals::frameTime) {
		camera.setDeltaTime(totalTime);
		//angle = (angle + step) > 2 * PI ? (angle + step) - 2 * PI : angle + step;
		totalTime = 0;
	}
	
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	switch (key)
	{
		case 'A': case 'a':
			camera.moveOx(-1);
			break;
		case 'D':case'd':
			camera.moveOx(1);
			break;
		case 'W':case 'w':
			camera.moveOz(-1);
			break;
		case 'S': case 's':
			camera.moveOz(1);
			break ;
		case 'Q':case 'q':
			camera.moveOy(1);
			break;
		case 'E': case 'e':
			camera.moveOy(-1);
			break;
		case 'R': case 'r':
			camera.rotateOy(-1);
			break;
		case 'T': case 't':
			camera.rotateOy(1);
			break;
		case 'Y': case 'y':
			camera.rotateOx(-1);
			break;
		case 'U': case 'u':
			camera.rotateOx(1);
			break;
		case 'I': case 'i':
			camera.rotateOz(-1);
			break;
		case 'O': case 'o':
			camera.rotateOz(1);
			break;
		default:
			break;
	}
}

void CleanUp()
{
	glDeleteBuffers(1, &modelIboId);
	glDeleteBuffers(1, &modelVboId);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Game Engine", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();


	printf("Press any key...\n");
	_getch();

	
	return 0;
}


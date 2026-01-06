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
#include <algorithm>
#include <string>
#include <cstring>

#define PI 3.14159265358979323846


GLuint vboId;
Shaders myShaders;

float angle = 0;
float step = 0.05;

float totalTime = 0;

Camera camera = Camera();


ResourceManager* resourceManager = ResourceManager::GetInstance();

void readNfgLine(std::string str,std::string field, std::vector<float> &numbers) {
	//std::cout << str << " ";
	int posIndex = str.find(field);
	std::cout << posIndex << "\n";
	if (posIndex != -1) {

		//std::cout <<"substring"<< str.substr(posIndex + field.length() + 1, str.find(';') - posIndex - field.length() - 1) << '\n';
		std::string numbersStr = str.substr(posIndex + field.length() + 1, str.find(';') - posIndex - field.length() - 1);
		std::stringstream ss(numbersStr);
		std::string token;

		while (std::getline(ss, token, ',')) {
			numbers.push_back(std::stof(token)); // convertim string -> float
		}
		//for (float f : numbers) {
		//	std::cout << field << f << " ";
		//}
		//std::cout << '\n';
	}
}

void readNfg(const char* filename, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
	std::ifstream file(filename);
	std::string str;
	while (std::getline(file, str))
	{	
		std::vector<float> numbers;

		readNfgLine(str, "pos:", numbers);

		//std::cout << numbers.size() << '\n';

		if (numbers.size() == 3) {
			Vertex v;
			v.pos.x = numbers[0];
			v.pos.y = numbers[1];
			v.pos.z = numbers[2];
			vertices.push_back(v);
		}
		
		numbers.clear();
		readNfgLine(str, "norm:", numbers);

		numbers.clear();
		readNfgLine(str, "binorm:", numbers);

		numbers.clear();
		readNfgLine(str, "tgt:", numbers);

		numbers.clear();
		readNfgLine(str, "uv:", numbers);

		int posIndex = str.find("NrIndices:");
		if (posIndex != std::string::npos) {
			std::string numberStr = str.substr(posIndex + 10);
			int nrIndices = std::stoi(numberStr);
			std::cout << "NrIndices: " << nrIndices << "\n";
			std::getline(file, str);

			int posIndex = str.find("]");

			std::string numbersStr = str.substr(posIndex + 3, str.find(';', posIndex + 3) - posIndex - 3);
			std::cout << str << "\n";
			std::stringstream ss(numbersStr);
			std::string token;

			std::vector<float> numbers;

			while (std::getline(ss, token, ',')) {
				numbers.push_back(std::stof(token)); // convertim string -> float
			}
			for (float f : numbers) {
				std::cout << "indice:" << f << " ";
			}
		}
	}
}


int Init ( ESContext *esContext )
{
	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );

	//triangle data (heap)
	//Vertex verticesData[3];

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	readNfg("C:/Users/Andrei/Desktop/Gameloft/NewTrainingFramework_2015/NewTrainingFramework/modelTest.nfg", vertices, indices);

	for (auto v : vertices) {
		std::cout << "Vertex pos: " << v.pos.x << ", " << v.pos.y << ", " << v.pos.z << "\n";
	}

	/*verticesData[0].pos.x =  0.0f;  verticesData[0].pos.y =  0.5f;  verticesData[0].pos.z =  0.0f;
	verticesData[1].pos.x = -0.5f;  verticesData[1].pos.y = -0.5f;  verticesData[1].pos.z =  0.0f;
	verticesData[2].pos.x =  0.5f;  verticesData[2].pos.y = -0.5f;  verticesData[2].pos.z =  0.0f;*/

	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].color.x = i % 2 ? 1.0f : 0.0f;
		vertices[i].color.y = i % 3 ? 1.0f : 0.0f;
		vertices[i].color.z = i % 4 ? 1.0f : 0.0f;
	}

	std::cout << "marime:"<<vertices.size() * sizeof(Vertex);

	//buffer object
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	resourceManager->Init();

	//creation of shaders and program 
	return myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");

}

void Draw ( ESContext *esContext )
{
	Matrix mRotation;

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(myShaders.program);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	
	if(myShaders.positionAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.positionAttribute);
		glVertexAttribPointer(myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (myShaders.colorAttribute != -1) {
		glEnableVertexAttribArray(myShaders.colorAttribute);
		glVertexAttribPointer(myShaders.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3)));
	}

	mRotation.SetRotationZ(angle);

	if (myShaders.matrixUniform != -1) {
		glUniformMatrix4fv(myShaders.matrixUniform, 1, GL_FALSE, (float*)mRotation.m);
	}

	Matrix MVP = camera.viewMatrix * camera.perspectiveMatrix;
	MVP = mRotation * MVP;

	if (myShaders.matrixCamera != -1) {
		glUniformMatrix4fv(myShaders.matrixCamera, 1, GL_FALSE, (float*)MVP.m);
	}

	// Draw after the uniform is set
	glDrawArrays(GL_TRIANGLE_FAN, 0, 8);


	glBindBuffer(GL_ARRAY_BUFFER, 0);

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
	
} 

void Update ( ESContext *esContext, float deltaTime )
{
	camera.setDeltaTime(deltaTime);
	//printf("Update: %f\n", deltaTime);
	totalTime += deltaTime;
	if (totalTime >= Globals::frameTime) {
		camera.setDeltaTime(totalTime);
		angle = (angle + step) > 2 * PI ? (angle + step) - 2 * PI : angle + step;
		totalTime = 0;
		deltaTime = Globals::frameTime;
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
		default:
			break;
	}
}

void CleanUp()
{
	glDeleteBuffers(1, &vboId);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

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


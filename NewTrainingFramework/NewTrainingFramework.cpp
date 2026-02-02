// NewTrainingFramework.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include "Vertex.h"
#include "Shaders.h"
#include "Globals.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "SceneObject.h"

#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

#include "ReadNFG.h"

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
SceneManager* sceneManager = SceneManager::GetInstance();

int Init ( ESContext *esContext )
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	int width, height, bpp;
	char *pixelArray = nullptr;

	resourceManager->Init();
	sceneManager->Init();

	int currentObj = 0;

	//for (auto i = 0; i < sceneManager->currentSceneObjects.size(); i++) {
	//	std::cout << "Object " << i << " Model: " << sceneManager->currentSceneObjects[i]->model->file << " Texture: " << sceneManager->currentSceneObjects[i]->texture->file << "\n";
	//}

	std::cout << sceneManager->currentSceneObjects[currentObj]->model->file << "\n";
	
	readNfg(std::string(sceneManager->currentSceneObjects[1]->model->file), vertices, indices);
	

	pixelArray = LoadTGA(std::string(sceneManager->currentSceneObjects[currentObj]->texture->file).c_str(), &width, &height, &bpp);
	std::cout << width << " " << height << " " << bpp << "\n";

	glGenBuffers(1, &modelVboId);
	glGenBuffers(1, &modelIboId);
	
	glBindBuffer(GL_ARRAY_BUFFER, modelVboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelIboId);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenTextures(1, &sceneManager->currentSceneObjects[currentObj]->texture->id);
	glBindTexture(sceneManager->currentSceneObjects[currentObj]->texture->type, idTexture);
	GLint format = (bpp == 32) ? GL_RGBA : GL_RGB;
	glTexImage2D(resourceManager->textureResources[4]->type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixelArray);

	glTexParameteri(sceneManager->currentSceneObjects[currentObj]->texture->type, GL_TEXTURE_WRAP_S, sceneManager->currentSceneObjects[currentObj]->texture->wrap_s);
	glTexParameteri(sceneManager->currentSceneObjects[currentObj]->texture->type, GL_TEXTURE_WRAP_T, sceneManager->currentSceneObjects[currentObj]->texture->wrap_t);

	glTexParameteri(sceneManager->currentSceneObjects[currentObj]->texture->type, GL_TEXTURE_MIN_FILTER, sceneManager->currentSceneObjects[currentObj]->texture->min_filter);
	glTexParameteri(sceneManager->currentSceneObjects[currentObj]->texture->type, GL_TEXTURE_MAG_FILTER, sceneManager->currentSceneObjects[currentObj]->texture->mag_filter);

	glBindTexture(sceneManager->currentSceneObjects[currentObj]->texture->type, 0);

	return modelShader.Init((char*)resourceManager->shaderResources[10]->vs.c_str(), (char *)resourceManager->shaderResources[10]->fs.c_str());
}

void Draw ( ESContext *esContext )
{
	Matrix mRotation;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	if (modelShader.uvAttribute != -1)
	{
		glEnableVertexAttribArray(modelShader.uvAttribute);
		glVertexAttribPointer(modelShader.uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, uv));
	}

	if (modelShader.colorAttribute != -1)
	{
		glEnableVertexAttribArray(modelShader.colorAttribute);
		glVertexAttribPointer(modelShader.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, color));
	}

	Matrix MVP = camera.viewMatrix * camera.perspectiveMatrix;

	if (modelShader.matrixCamera != -1) {
		glUniformMatrix4fv(modelShader.matrixCamera, 1, GL_FALSE, (float*)MVP.m);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(resourceManager->textureResources[4]->type, idTexture);

	glDrawElements(GL_TRIANGLES,indices.size(), GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
} 

void Update ( ESContext *esContext, float deltaTime )
{
	totalTime += deltaTime;
	if (totalTime >= Globals::frameTime) {
		camera.setDeltaTime(totalTime);
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


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
#include "ReadNFG.h"

#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

int currentObj = 1;

float totalTime = 0;

ResourceManager* resourceManager = ResourceManager::GetInstance();
SceneManager* sceneManager = SceneManager::GetInstance();
Camera camera = Camera();

int Init ( ESContext *esContext )
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	resourceManager->Init();
	sceneManager->Init();
	
	return 0;
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
	SceneObject* obj = sceneManager->currentSceneObjects[currentObj];

	glDeleteBuffers(1, &obj->model->iboId);
	glDeleteBuffers(1, &obj->model->iboId);
}

void Draw(ESContext* esContext) {
	SceneManager::GetInstance()->Draw(esContext,camera);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	ESContext esContext;

    esInitContext ( &esContext );

	sceneManager->InitWindow(&esContext);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw);
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();


	printf("Press any key...\n");
	_getch();

	return 0;
}


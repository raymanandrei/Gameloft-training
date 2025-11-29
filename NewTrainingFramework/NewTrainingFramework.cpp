// NewTrainingFramework.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include <conio.h>
#include "Globals.h"
#include "Camera.h"


constexpr double PI = 3.14159265358979323846;

GLuint vboId;
Shaders myShaders;

float angle = 0;
float step = 0.1;

float totalTime = 0;

Camera camera = Camera();

int Init ( ESContext *esContext )
{
	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );

	//triangle data (heap)
	Vertex verticesData[3];

	verticesData[0].pos.x =  0.0f;  verticesData[0].pos.y =  0.5f;  verticesData[0].pos.z =  0.0f;
	verticesData[1].pos.x = -0.5f;  verticesData[1].pos.y = -0.5f;  verticesData[1].pos.z =  0.0f;
	verticesData[2].pos.x =  0.5f;  verticesData[2].pos.y = -0.5f;  verticesData[2].pos.z =  0.0f;

	verticesData[0].color.x = 0.0; verticesData[1].color.x = 0.0; verticesData[2].color.x = 1.0;
	verticesData[0].color.y = 0.0; verticesData[1].color.y = 1.0; verticesData[2].color.y = 0.0;
	verticesData[0].color.z = 1.0;  verticesData[1].color.z = 0.0; verticesData[2].color.z = 0.0;


	//buffer object
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

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

	Matrix proj = camera.getPerspectiveMatrix();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			printf("%f ", proj.m[i][j]);
		printf("\n");
	}

	//Matrix model;

	//model.SetIdentity();

	//Matrix MVP = model * camera.getViewMatrix() * camera.getPerspectiveMatrix();

	//glUniformMatrix4fv(myShaders.matrixUniform, 1, GL_FALSE, &MVP.m[0][0]);

	//glDrawArrays(GL_TRIANGLES, 0, 3);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );

	//mRotation.SetRotationZ(angle);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);

	mRotation.SetRotationZ(angle);


	if (myShaders.matrixUniform != -1) {
		glUniformMatrix4fv(myShaders.matrixUniform, 1, GL_FALSE, (float*)mRotation.m);
	}

	
}

void Update ( ESContext *esContext, float deltaTime )
{
	totalTime += deltaTime;
	if (totalTime > Globals::frameTime) {
		if (angle > 2 * PI) angle = 0;
		else angle = angle + 0.1;
		totalTime = totalTime - Globals::frameTime;
		deltaTime - Globals::frameTime;
	}
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	switch (key)
	{
		case 'A': case 'a':
			printf("A");
			camera.moveOx(-1);
			break;
		case 'S': case 's':
			printf("S");
			camera.moveOy(-1);
			break ;
		case 'D':case'd':
			printf("D");
			camera.moveOx(1);
			break;
		case 'W':case 'w':
			printf("W");
			camera.moveOy(1);
			break;
		case 'Q':case 'q':
			printf("Q");
			camera.rotateOz(1);
			break;
		case 'E': case 'e':
			printf("E");
			camera.rotateOz(-1);
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


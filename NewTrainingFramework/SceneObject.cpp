#include "stdafx.h"
#include "SceneObject.h"
#include "Camera.h"
#include "Shaders.h"
#include "Vertex.h"

SceneObject::SceneObject() {
}

void sendCommonData(ESContext* esContext,Shaders modelShader) {

	Camera camera = Camera();
	Matrix mRotation;

	GLuint modelVboId;
	GLuint modelIboId;
	GLuint idTexture;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, modelVboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelIboId);

	if (modelShader.positionAttribute != -1)
	{
		glEnableVertexAttribArray(modelShader.positionAttribute);
		glVertexAttribPointer(modelShader.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (modelShader.textureUniform != -1)
	{
		glUniform1i(modelShader.textureUniform, 0);
	}

	if (modelShader.uvAttribute != -1)
	{
		glEnableVertexAttribArray(modelShader.uvAttribute);
		glVertexAttribPointer(modelShader.uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, uv));
	}


	Matrix MVP = camera.viewMatrix * camera.perspectiveMatrix;

	if (modelShader.matrixCamera != -1) {
		glUniformMatrix4fv(modelShader.matrixCamera, 1, GL_FALSE, (float*)MVP.m);
	}

	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(resourceManager->textureResources[4]->type, idTexture);


	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);

}

void sendSpecificData() {

}

void SceneObject::Draw(ESContext* esContext) {

	Shaders modelShader;
	glUseProgram(modelShader.program);

	sendCommonData(esContext,modelShader);
}

void SceneObject::Update(float deltaTime) {
}	
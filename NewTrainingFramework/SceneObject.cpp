#include "stdafx.h"
#include <iostream>
#include "SceneObject.h"
#include "Camera.h"
#include "Shaders.h"
#include "Vertex.h"

SceneObject::SceneObject() {
}

void SceneObject::sendCommonData(ESContext* esContext) {


	glBindBuffer(GL_ARRAY_BUFFER,this->model->vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->model->iboId);

	if (this->shader->sr->positionAttribute != -1){
		glEnableVertexAttribArray(this->shader->sr->positionAttribute);
		glVertexAttribPointer(this->shader->sr->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (this->shader->sr->textureUniform != -1){
		glUniform1i(this->shader->sr->textureUniform, 0);
	}

	if (this->shader->sr->uvAttribute != -1){
		glEnableVertexAttribArray(this->shader->sr->uvAttribute);
		glVertexAttribPointer(this->shader->sr->uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, uv));
	}

	MVP = camera.viewMatrix * camera.perspectiveMatrix;

	if (this->shader->sr->matrixCamera != -1) {
		glUniformMatrix4fv(this->shader->sr->matrixCamera, 1, GL_FALSE, (float*)MVP.m);
	}

	if (this->shader->sr->colorVarying != -1) {
		glEnableVertexAttribArray(this->shader->sr->colorVarying);
		glVertexAttribPointer(this->shader->sr->colorVarying, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, color));
	}

	if (this->texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(this->texture->tr->type, this->texture->tr->id);
	}

	glDrawElements(GL_TRIANGLES, this->model->nrIndici, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void sendSpecificData() {

}

void SceneObject::Draw(ESContext* esContext) {
	
	glUseProgram(this->shader->programId);

	sendCommonData(esContext);

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);

}

void SceneObject::Update(float deltaTime) {
}	
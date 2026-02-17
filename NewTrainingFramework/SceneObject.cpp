#include "stdafx.h"
#include <iostream>
#include "SceneObject.h"
#include "Camera.h"
#include "Shaders.h"
#include "Vertex.h"

SceneObject::SceneObject() {
}

void SceneObject::sendCommonData(ESContext* esContext, Camera camera) {

	//std::cout << "Sending common data for object ID: " << this->id << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER,this->model->vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->model->iboId);
	//std::cout << "Bound VBO ID: " << this->model->vboId << " and IBO ID: " << this->model->iboId << std::endl;

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

	if (this->shader->sr->colorAttribute != -1) {
		glEnableVertexAttribArray(this->shader->sr->colorAttribute);
		glVertexAttribPointer(this->shader->sr->colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, color));
	}

	if (this->texture){
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(this->texture->tr->type, this->texture->tr->id);
	}

	//std::cout << "Drawing object with " << this->model->nrIndici << " indices." << std::endl;
	glDrawElements(GL_TRIANGLES, this->model->nrIndici, GL_UNSIGNED_SHORT, 0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void sendSpecificData() {

}

void SceneObject::Draw(ESContext* esContext,Camera camera) {
	
	glUseProgram(this->shader->programId);

	sendCommonData(esContext,camera);

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);

}

void SceneObject::Update(float deltaTime) {
}	
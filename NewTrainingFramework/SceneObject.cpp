#include "stdafx.h"
#include <iostream>
#include "SceneObject.h"
#include "Camera.h"
#include "Shaders.h"
#include "Vertex.h"

SceneObject::SceneObject() {
	//camera = Camera();
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

	if (this->shader->sr->uvAttribute != -1){
		glEnableVertexAttribArray(this->shader->sr->uvAttribute);
		glVertexAttribPointer(this->shader->sr->uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, uv));
	}
	
	if (this->shader->sr->uv2Attribute != -1) {
		glEnableVertexAttribArray(this->shader->sr->uv2Attribute);
		glVertexAttribPointer(this->shader->sr->uv2Attribute,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(const GLvoid *)offsetof(Vertex,uv2));
	}

	MVP = camera.viewMatrix * camera.perspectiveMatrix;

	if (this->shader->sr->matrixCamera != -1) {
		glUniformMatrix4fv(this->shader->sr->matrixCamera, 1, GL_FALSE, (float*)MVP.m);
	}

	if (this->shader->sr->colorAttribute != -1) {
		glEnableVertexAttribArray(this->shader->sr->colorAttribute);
		glVertexAttribPointer(this->shader->sr->colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, color));
	}

	if (this->shader->sr->objectColor != -1) {
		glUniform3f(this->shader->sr->objectColor, this->color.x, this->color.y, this->color.z);
	}

	if (this->shader->sr->u_height != -1) {
		//std::cout << this->color.x << this->color.y << this->color.z << '\n';
		glUniform3f(this->shader->sr->u_height, this->color.x, this->color.y, this->color.z);
	}

	if (this->texture.size()) {

		for (int i = 0; i < this->texture.size();i++) {
			//std::cout << "Texture with id:" << this->texture[i]->tr->id << "and file:" << this->texture[i]->tr->file << '\n';
			glUniform1i(GL_TEXTURE0 + i, this->texture[i]->tr->id);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(this->texture[i]->tr->type, this->texture[i]->tr->id);
		}		
	}

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


#include "stdafx.h"
#include <iostream>
#include "SkyBox.h"
#include "SceneManager.h"
#include "Vertex.h"

void SkyBox::Update() {
	//std::cout << this->position.x << " " << this->position.y << " " << this->position.z << '\n';
	SceneManager* sceneManager = SceneManager::GetInstance();
	if (this->followingCamera.x || this->followingCamera.y || this->followingCamera.z) {
		this->position.x = sceneManager->camera.position.x;
		this->position.y = sceneManager->camera.position.y;
		this->position.z = sceneManager->camera.position.z;
	}
}

void SkyBox::sendCommonData(ESContext* esContext){

	//std::cout << SceneManager::GetInstance()->camera.position.x << " " << SceneManager::GetInstance()->camera.position.y << " " << SceneManager::GetInstance()->camera.position.z << std::endl;
	//std::cout << "Sending common data for object ID: " << this->id << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, this->model->vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->model->iboId);
	//std::cout << "Bound VBO ID: " << this->model->vboId << " and IBO ID: " << this->model->iboId << std::endl;

	if (this->shader->sr->positionAttribute != -1) {
		glEnableVertexAttribArray(this->shader->sr->positionAttribute);
		glVertexAttribPointer(this->shader->sr->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (this->shader->sr->uvAttribute != -1) {
		glEnableVertexAttribArray(this->shader->sr->uvAttribute);
		glVertexAttribPointer(this->shader->sr->uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, uv));
	}

	Matrix MVP = MVP.SetIdentity() * SceneManager::GetInstance()->camera.viewMatrix * SceneManager::GetInstance()->camera.perspectiveMatrix;

	MVP = Matrix().SetTranslation(this->position.x, this->position.y, this->position.z) * MVP;

	MVP = Matrix().SetScale(this->scale.x, this->scale.y, this->scale.z) * MVP;

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

	if (this->texture.size()) {

		for (int i = 0; i < this->texture.size(); i++) {
			//std::cout << "Texture with id:" << this->texture[i]->tr->id << "and file:" << this->texture[i]->tr->file << '\n';
			glUniform1i(this->shader->sr->textureUniform[i], i);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(this->texture[i]->tr->type, this->texture[i]->tr->id);
		}
	}

	glDrawElements(GL_TRIANGLES, this->model->nrIndici, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
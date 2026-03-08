#include "stdafx.h"
#include <iostream>
#include "Terrain.h"
#include "Vertex.h"
#include "SceneManager.h"

Terrain::Terrain() {
	this->cellSize = 500;
	this->nrCells = 4;
	this->blendTextureOffset = Vector2(0.0f, 0.0f);
}

void Terrain::sendCommonData(ESContext* esContext) {

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

	if (this->shader->sr->uv2Attribute != -1) {
		glEnableVertexAttribArray(this->shader->sr->uv2Attribute);
		glVertexAttribPointer(this->shader->sr->uv2Attribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, uv2));
	}

	if (this->shader->sr->blendTextureOffset != -1) {
		glUniform2f(
			this->shader->sr->blendTextureOffset,
			blendTextureOffset.x,
			blendTextureOffset.y
		);
	}

	//std::cout << "Blend Texture Offset: " << blendTextureOffset.x << " " << blendTextureOffset.y << std::endl;
	Matrix MVP = MVP.SetIdentity() * SceneManager::GetInstance()->camera.viewMatrix * SceneManager::GetInstance()->camera.perspectiveMatrix;

	MVP = Matrix().SetTranslation(this->position.x, this->position.y, this->position.z) * MVP;

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

void Terrain::Update() {
	SceneManager* sceneManager = SceneManager::GetInstance();
	int dx = sceneManager->camera.position.x - this->position.x;
	int dz = sceneManager->camera.position.z - this->position.z;
	float textureStep = 1.0 / this->nrCells;

	//std::cout << this->model->mr->id << " " << dx << " " << dz << std::endl;
	//std::cout << textureStep << std::endl;
	if (dx > this->cellSize)
	{
		this->position.x += this->cellSize;
		this->blendTextureOffset.x -= textureStep;
		std::cout << this->blendTextureOffset.x << std::endl;
	}
	else if (dx < -this->cellSize)
	{
		this->position.x -= this->cellSize;
		this->blendTextureOffset.x += textureStep;
	}
	if (dz > this->cellSize)
	{
		this->position.z += this->cellSize;
		this->blendTextureOffset.y -= textureStep;
	}
	else if (dz < -this->cellSize)
	{
		this->position.z -= this->cellSize;
		this->blendTextureOffset.y += textureStep;
	}

	//std::cout << dx << " " << dz << std::endl;
}	
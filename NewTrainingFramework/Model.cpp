#include "stdafx.h"
#include <vector>
#include "Model.h"
#include "Vertex.h"
#include "ReadNFG.h"

#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

Model::Model() {
}

Model::~Model() {
}

bool Model::Load() {
	glGenBuffers(1, &iboId);
	glGenBuffers(1, &vboId);

	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	readNfg(mr->file,vertices,indices);

	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].color.x = 1.0f;
		vertices[i].color.y = 0.0f;
		vertices[i].color.z = 1.0f;
	}

	nrIndici = indices.size();
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}	
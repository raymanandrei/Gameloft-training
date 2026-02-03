#include "stdafx.h"
#include "Texture.h"

#include "../Utilities/utilities.h"

Texture::Texture() {
}

Texture::~Texture() {
}

bool Texture::Load() {

	int width, height, bpp;
	char* pixelArray = nullptr;

	pixelArray = LoadTGA((tr->file).c_str(), &width, &height, &bpp);

	//std::cout << width << " " << height << " " << bpp << "\n";

	glGenTextures(1, &textureId);
	glBindTexture(tr->type, textureId);

	GLint format = (bpp == 32) ? GL_RGBA : GL_RGB;

	glTexImage2D(tr->type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixelArray);

	glTexParameteri(tr->type, GL_TEXTURE_WRAP_S, tr->wrap_s);
	glTexParameteri(tr->type, GL_TEXTURE_WRAP_T, tr->wrap_t);

	glTexParameteri(tr->type, GL_TEXTURE_MIN_FILTER, tr->min_filter);
	glTexParameteri(tr->type, GL_TEXTURE_MAG_FILTER,tr->mag_filter);

	glBindTexture(tr->type, 0);

	return true;
}


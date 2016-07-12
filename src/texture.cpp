#include "texture.hpp"

#include <cstdio>
#include "..\lib\glew\glew.h"

unsigned int loadBitmap(const char* filename)
{
	FILE * bitmapFile;
	fopen_s(&bitmapFile, filename, "rb");
	char header[54];

	if (!bitmapFile)
		return 0;
	fread(header, 1, 54, bitmapFile);

	if (header[0] != 'B' || header[1] != 'M')
		return 0;
	if (*(int*)&(header[0x1E]) != 0)
		return 0;
	if (*(int*)&(header[0x1C]) != 24)
		return 0;


	int dataPos		= *(int*)&(header[0x0A]);
	int imageSize	= *(int*)&(header[0x22]);
	int width		= *(int*)&(header[0x12]);
	int height		= *(int*)&(header[0x16]);

	if (!imageSize)
		imageSize = width * height * 3;
	if (!dataPos)
		dataPos = 54;

	char* buffer = new char[imageSize];

	fread(buffer, 1, imageSize, bitmapFile);

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, buffer);

	delete [] buffer;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureID;
}

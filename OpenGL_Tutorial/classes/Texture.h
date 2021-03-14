#pragma once

#include <glad/glad.h>
#include <iostream>

enum class TextureType: int
{
	RGB = GL_RGB,
	RGBA = GL_RGBA,
};

class Texture
{
public:
	Texture(const char* texturePath, TextureType textureType = TextureType::RGB);
	~Texture();

	void use();
	unsigned ID();
private:
	unsigned textureID;
};


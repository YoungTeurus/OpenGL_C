#pragma once

#include <glad/glad.h>
#include <iostream>

class Texture
{
public:
	Texture(const char* texturePath);
	~Texture();

	void use();
	unsigned ID();
private:
	unsigned textureID;
};


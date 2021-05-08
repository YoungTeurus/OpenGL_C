#pragma once

#include <glad/glad.h>
#include <iostream>
#include <map>

enum class TextureType : unsigned {
	DIFFUSE = 0,
	SPECULAR = 1
};

static std::map<TextureType, std::string> textureTypeShaderNames = {
	{TextureType::DIFFUSE, "texture_diffuse"},
	{TextureType::SPECULAR, "texture_specular"},
};

class Texture
{
public:
	TextureType type;
	std::string filename;
private:
	unsigned textureID;

public:
	Texture(TextureType textureType);
	Texture(const char* pathToTexture, TextureType textureType);
	~Texture();

	void loadFromFileAndSetTextureID(const char* pathToTexture);

	unsigned ID() const;
};


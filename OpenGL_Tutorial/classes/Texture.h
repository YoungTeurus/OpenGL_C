#pragma once

#include <glad/glad.h>
#include <iostream>
#include <map>

enum class TextureType : unsigned {
	DIFFUSE = 0,
	SPECULAR = 1,
	NORMAL = 2,
	HEIGHT = 3
};

static std::map<TextureType, std::string> textureTypeShaderNames = {
	{TextureType::DIFFUSE, "texture_diffuse"},
	{TextureType::SPECULAR, "texture_specular"},
	{TextureType::NORMAL, "texture_normal"},
	{TextureType::HEIGHT, "texture_height"}
};

static std::map<std::string, TextureType> shaderNamesTextureType = {
	{"texture_diffuse", TextureType::DIFFUSE},
	{"texture_specular", TextureType::SPECULAR},
	{"texture_normal", TextureType::NORMAL},
	{"texture_height", TextureType::HEIGHT}
};

class Texture
{
public:
	TextureType type;
	std::string filename;
private:
	unsigned int textureID;

public:
	Texture(TextureType textureType);
	Texture(const char* pathToTexture, TextureType textureType);
	Texture(const char* filename, const std::string& directory, TextureType textureType);
	~Texture();

	void loadFromFileAndSetTextureID(const char* pathToTexture);

	unsigned ID() const;
};


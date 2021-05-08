#pragma once

#include <glad/glad.h>
#include <iostream>
#include <map>

enum class TextureRGBMode: int
{
	RGB = GL_RGB,
	RGBA = GL_RGBA,
};

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
private:
	unsigned textureID;

public:
	Texture(const char* texturePath, TextureType textureType, TextureRGBMode textureRGBMode = TextureRGBMode::RGB);
	~Texture();

	unsigned ID() const;
};


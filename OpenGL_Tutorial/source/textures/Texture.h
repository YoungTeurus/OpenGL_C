#pragma once
#include <map>
#include <string>

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

class Texture
{
public:
	unsigned int id;
	TextureType type;
	std::string path;
};
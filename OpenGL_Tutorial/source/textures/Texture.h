#pragma once

#include <map>
#include <string>
#include <iostream>

enum class TextureType : unsigned {
	UNDEFINED = 0,
	DIFFUSE = 1,
	SPECULAR = 2,
	NORMAL = 3,
	HEIGHT = 4,
	CUBE_MAP = 5,
	FONT_GLYPH = 6
};

static std::map<TextureType, std::string> textureTypeShaderNames = {
	{TextureType::DIFFUSE, "texture_diffuse"},
	{TextureType::SPECULAR, "texture_specular"},
	{TextureType::NORMAL, "texture_normal"},
	{TextureType::HEIGHT, "texture_height"}
};

class Texture
{
private:
	void setSTBIFilpState(const bool flipState);
protected:
	unsigned int id;
	TextureType type = TextureType::UNDEFINED;
	std::string directory;
	std::string fileName;
	
	bool isInitialized = false;

	void setSTBIFilp(const bool flipState);

	virtual void loadTextureAndSetFields(const std::string& fileName, const std::string& directory);

	unsigned char* loadImageData(std::string pathToFile, int* width, int*height, int* nrComponents);
	
	void freeSTBIImageData(unsigned char* data);
	
public:

	Texture() = default;

	void loadFromFile(const std::string& fileName, const std::string& directory, const bool isUV_flipped = false)
		{
			if(isInitialized)
		{
			std::cout << "Texture::loadFromFile: There was an attempt to reload texture '" << directory << "/" << fileName << "'." << std::endl;
			return;
		}

		if (isUV_flipped)
		{
			setSTBIFilpState(true);
		}

		loadTextureAndSetFields(fileName, directory);

		if (isUV_flipped)
		{
			setSTBIFilpState(false);
		}
	}

	void setType(TextureType type)
	{
		this->type = type;
	}

	unsigned getId() const
	{
		if (!isInitialized)
		{
			throw std::exception("Texture::getId: texture was used before initialization!");
		}
		return id;
	}

	bool getIsInitialized() const
	{
		return isInitialized;
	}

	TextureType getType() const
	{
		return type;
	}

	std::string getDirectory() const
	{
		return directory;
	}

	std::string getFileName() const
	{
		return fileName;
	}
};
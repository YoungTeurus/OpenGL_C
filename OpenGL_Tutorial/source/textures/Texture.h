#pragma once
#include <map>
#include <string>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#endif


enum class TextureType : unsigned {
	UNDEFINED = 0,
	DIFFUSE = 1,
	SPECULAR = 2,
	NORMAL = 3,
	HEIGHT = 4
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
	bool isInitialized = false;
public:
	unsigned int id;
	TextureType type;
	std::string path;
	std::string name;

	Texture()
	{
	}

	void loadFromFile2DTexture(const std::string& fileName, const std::string& pathToFolder, TextureType type)
	{
		if(isInitialized)
		{
			return;
		}

		this->name = fileName;
		this->path = pathToFolder;
		this->type = type;

		string pathToFile = pathToFolder + "/" + fileName;

	    glGenTextures(1, &id);

	    int width, height, nrComponents;
	    unsigned char* data = stbi_load(pathToFile.c_str(), &width, &height, &nrComponents, 0);
	    if (data)
	    {
	        GLenum format;
	        if (nrComponents == 1)
	            format = GL_RED;
	        else if (nrComponents == 3)
	            format = GL_RGB;
	        else if (nrComponents == 4)
	            format = GL_RGBA;

	        glBindTexture(GL_TEXTURE_2D, id);
	        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	        glGenerateMipmap(GL_TEXTURE_2D);

	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	    	isInitialized = true;
	    }
	    else
	    {
	        std::cout << "Texture failed to load at path: " << pathToFile << std::endl;
	    }
	    stbi_image_free(data);
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
};
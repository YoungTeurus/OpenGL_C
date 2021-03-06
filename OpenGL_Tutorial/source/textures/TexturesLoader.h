#pragma once
#include <map>
#include <string>


#include "CubeMap.h"
#include "Texture.h"
#include "../utility/FilePaths.h"

class TexturesLoader
{
private:
	static TexturesLoader* instance;

	std::map<std::string, Texture*>* loadedTextures;

	TexturesLoader()
	{
		loadedTextures = new std::map<std::string, Texture*>();
	}
	
	void addTexture(Texture* texture)
	{
		const std::string key = texture->getDirectory() + FilePaths::getSplitter() + texture->getFileName();
		loadedTextures->insert(
			std::pair<std::string, Texture*>(key, texture)
		);
	}

public:
	static TexturesLoader* getInstance()
	{
		if(instance == nullptr)
		{
			instance = new TexturesLoader();
		}
		return instance;
	}

	Texture* get(const std::string& fileName) const
	{
		get(fileName, FilePaths::getPathToTexturesFolder());
	}

	Texture* get(const std::string& fileName, const std::string& directory) const
	{
		const std::string key = directory + FilePaths::getSplitter() + fileName;
		
		const auto it = loadedTextures->find(key);
		if(it != loadedTextures->end())
		{
			return it->second;
		}
		return nullptr;
	}

	Texture* getOrLoad2DTexture(const std::string& fileName, const bool isUV_flipped = false)
	{
		return getOrLoad2DTexture(fileName, FilePaths::getPathToTexturesFolder(), isUV_flipped);
	}

	Texture* getOrLoad2DTexture(const std::string& fileName, const std::string& directory, const bool isUV_flipped = false)
	{
		Texture* texture = get(fileName, directory);
		if (texture == nullptr)
		{
			texture = new Texture();
			texture->loadFromFile(fileName, directory, isUV_flipped);
			addTexture(texture);
		}
		return texture;
	}

	CubeMap* getOrLoadCubeMap(const std::string& fileName, const std::string& directory, const bool isUV_flipped = false)
	{
		CubeMap* returnCubeMap;
		Texture* texture = get(fileName, directory);
		if (texture == nullptr)
		{
			returnCubeMap = new CubeMap();
			returnCubeMap->loadFromFile(fileName, directory, isUV_flipped);
			addTexture(returnCubeMap);
		} else {
			returnCubeMap = new CubeMap(*texture);
		}

		return returnCubeMap;
	}
};

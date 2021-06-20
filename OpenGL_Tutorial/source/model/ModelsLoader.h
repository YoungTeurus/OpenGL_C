#pragma once
#include <map>

#include "Model.h"

class ModelsLoader
{
private:
	static ModelsLoader* instance;
	
	std::map<std::string, Model*>* loadedModels;

	ModelsLoader()
	{
		loadedModels = new std::map<std::string, Model*>();
	}

	Model* load(const std::string& pathToModel, const bool& isUVFlipped)
	{
		Model* newShader = new Model(FilePaths::getPathToModel(pathToModel), isUVFlipped);

		loadedModels->insert(std::pair<std::string, Model*>(pathToModel, newShader));
		
		return newShader;
	}
public:
	static ModelsLoader* getInstance()
	{
		if (instance == nullptr)
		{
			instance = new ModelsLoader();
		}
		return instance;
	}

	Model* getOrLoad(const std::string& pathToModel, const bool& isUVFlipped)
	{
		const auto it = loadedModels->find(pathToModel);
		if(it != loadedModels->end())
		{
			return it->second;
		}
		return load(pathToModel, isUVFlipped);
	}
};

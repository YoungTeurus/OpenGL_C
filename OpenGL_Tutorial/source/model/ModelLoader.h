#pragma once
#include <map>
#include <string>

#include "Model.h"

using namespace std;

class ModelLoader
{
private:
	map<string, Model*> loadedModels;
	vector<Texture> loadedTextures;
public:
	void loadModel(const string& modelName, const string& pathToModel, bool isUVFlipped = false)
	{
		Model* newModel = new Model(pathToModel, loadedTextures, isUVFlipped);
		pair<string, Model*> newPair(modelName, newModel);
		
		loadedModels.insert(newPair);
	}

	Model* getModel(const string& modelName)
	{
		const auto it =  loadedModels.find(modelName);
		if(it != loadedModels.end())
		{
			return it->second;
		}
		return nullptr;
	}
};
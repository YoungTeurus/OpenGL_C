#pragma once
#include <map>

#include "Shader.h"
#include "../utility/FilePaths.h"

class ShaderLoader
{
private:
	static ShaderLoader* instance;
	
	std::map<std::string, Shader*>* loadedShaders;

	ShaderLoader()
	{
		loadedShaders = new std::map<std::string, Shader*>();
	}
	
	Shader* load(const std::string& name, const bool& withGeometry = false)
	{
		return load(name, FilePaths::getPathToShaderFolderWithTrailingSplitter(), withGeometry);
	}

	Shader* load(const std::string& name, const std::string& pathToShaderFiles, const bool& withGeometry = false)
	{
		Shader* newShader = new Shader(name, pathToShaderFiles, withGeometry);

		loadedShaders->insert(std::pair<std::string, Shader*>(name, newShader));
		
		return newShader;
	}
public:
	static ShaderLoader* getInstance()
	{
		if (instance == nullptr)
		{
			instance = new ShaderLoader();
		}
		return instance;
	}


	Shader* get(const std::string& name) const
	{
		const auto it = loadedShaders->find(name);
		if(it != loadedShaders->end())
		{
			return it->second;
		}
		return nullptr;
	}

	Shader* reload(const std::string& name, const bool& withGeometry = false)
	{
		return load(name, FilePaths::getPathToShaderFolderWithTrailingSplitter(), withGeometry);
	}

	Shader* getOrLoad(const std::string& name, const bool& withGeometry = false)
	{
		return getOrLoad(name, FilePaths::getPathToShaderFolderWithTrailingSplitter(), withGeometry);
	}

	Shader* getOrLoad(const std::string& name, const std::string& pathToShaderFiles, const bool& withGeometry = false)
	{
		const auto it = loadedShaders->find(name);
		if(it != loadedShaders->end())
		{
			return it->second;
		}
		return load(name, pathToShaderFiles, withGeometry);
	}
};

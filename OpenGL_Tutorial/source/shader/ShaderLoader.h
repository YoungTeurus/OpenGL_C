#pragma once
#include <map>
#include <string>

using namespace std;

class ShaderLoader
{
private:
	map<string, Shader*> loadedShaders;
public:
	void loadShader(const string& shaderName, const string& shaderFileName, const string& pathToShader, bool withGeometry = false)
	{
		Shader* newShader = new Shader(shaderFileName, pathToShader, withGeometry);
		pair<string, Shader*> newPair(shaderName, newShader);

		loadedShaders.insert(newPair);
	}

	Shader* getShader(const string& shaderName)
	{
		const auto it =  loadedShaders.find(shaderName);
		if(it != loadedShaders.end())
		{
			return it->second;
		}
		return nullptr;
	}
};
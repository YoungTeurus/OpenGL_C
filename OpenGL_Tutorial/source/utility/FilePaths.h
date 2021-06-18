#pragma once

#include <string>

using namespace std;

class FilePaths
{
private:
	static string resourcesFolderName;
	static string modelsFolderName;
	static string shadersFolderName;
	static string texturesFolderName;

	static string splitter;
public:
	static string getPathToModel(const string& modelName)
	{
		return resourcesFolderName + splitter + modelsFolderName + splitter + modelName;
	}

	static string getPathToShader(const string& shaderName)
	{
		return getPathToShaderFolderWithTrailingSplitter() + shaderName;
	}

	static string getPathToShaderFolderWithTrailingSplitter()
	{
		return resourcesFolderName + splitter + shadersFolderName + splitter;
	}

	static string getPathToTexture(const string& textureName)
	{
		return getPathToTexturesFolderWithTrailingSplitter() + textureName;
	}

	static string getPathToTexturesFolderWithTrailingSplitter()
	{
		return resourcesFolderName + splitter + texturesFolderName + splitter;
	}
};

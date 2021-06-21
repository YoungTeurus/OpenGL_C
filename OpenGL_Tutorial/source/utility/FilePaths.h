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
	static string fontsFolderName;
	static string levelsFolderName;

	static string splitter;
public:
	static string getSplitter()
	{
		return splitter;
	}
	
	static string getPathToModel(const string& modelName)
	{
		return resourcesFolderName + splitter + modelsFolderName + splitter + modelName;
	}

	static string getPathToShader(const string& shaderName)
	{
		return getPathToShadersFolderWithTrailingSplitter() + shaderName;
	}

	static string getPathToShadersFolderWithTrailingSplitter()
	{
		return getPathToShadersFolder() + splitter;
	}

	static string getPathToShadersFolder()
	{
		return resourcesFolderName + splitter + shadersFolderName;
	}

	static string getPathToTexture(const string& textureName)
	{
		return getPathToTexturesFolderWithTrailingSplitter() + textureName;
	}

	static string getPathToTexturesFolderWithTrailingSplitter()
	{
		return getPathToTexturesFolder() + splitter;
	}

	static string getPathToTexturesFolder()
	{
		return resourcesFolderName + splitter + texturesFolderName;
	}

	static string getPathToFont(const string& fontName)
	{
		return getPathToFontsFolderWithTrailingSplitter() + fontName;
	}

	static string getPathToFontsFolderWithTrailingSplitter()
	{
		return getPathToFontsFolder() + splitter;
	}

	static string getPathToFontsFolder()
	{
		return resourcesFolderName + splitter + fontsFolderName;
	}

	static string getPathToLevelsFolder()
	{
		return resourcesFolderName + splitter + levelsFolderName;
	}
};

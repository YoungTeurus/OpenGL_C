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
		return getPathToShaderFolderWithTrailingSplitter() + shaderName;
	}

	static string getPathToShaderFolderWithTrailingSplitter()
	{
		return getPathToShaderFolder() + splitter;
	}

	static string getPathToShaderFolder()
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
		return getPathToFontFolderWithTrailingSplitter() + fontName;
	}

	static string getPathToFontFolderWithTrailingSplitter()
	{
		return getPathToFontFolder() + splitter;
	}

	static string getPathToFontFolder()
	{
		return resourcesFolderName + splitter + fontsFolderName;
	}
};

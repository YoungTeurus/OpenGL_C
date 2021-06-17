#include <string>

using namespace std;

class FilePaths
{
private:
	static const string resourcesFolderName;
	static const string modelsFolderName;
	static const string shadersFolderName;
	static const string texturesFolderName;

	static const string splitter;
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

const string FilePaths::resourcesFolderName = "resources";
const string FilePaths::modelsFolderName = "models";
const string FilePaths::shadersFolderName = "shaders";
const string FilePaths::texturesFolderName = "textures";
const string FilePaths::splitter = R"(/)";
#include "CubeMap.h"

#include <glad/glad.h>
#include "../utility/FilePaths.h"

std::vector<std::string> CubeMap::fileSuffixes = {
	"",					// R"(skybox/right.jpg)",
	"_left",			// R"(skybox/left.jpg)",
	"_top",				// R"(skybox/top.jpg)",
	"_bottom",			// R"(skybox/bottom.jpg)",
	"_front",			// R"(skybox/front.jpg)",
	"_back"				// R"(skybox/back.jpg)",
};

void CubeMap::loadTextureAndSetFields(const std::string& fileName, const std::string& directory)
{
	this->fileName = fileName;
	this->directory = directory;
	this->type = TextureType::CUBE_MAP;

	const std::string pathToMainFile = directory + FilePaths::getSplitter() + fileName;

	size_t indexOfLastDot = pathToMainFile.find_last_of('.');

	const std::string pathToMainFileToSuffix = pathToMainFile.substr(0, indexOfLastDot);
	const std::string filesExtension = pathToMainFile.substr(indexOfLastDot);

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	int width, height, nrChannels;
	unsigned char* data;
	for(unsigned i = 0; i < 6; i++)
	{
		string pathToTexture = pathToMainFileToSuffix + fileSuffixes[i] + filesExtension;
		data = loadImageData(pathToTexture, &width, &height, &nrChannels);
		if (data){
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			isInitialized = true;
		} else {
			std::cout << "Cubemap tex failed to load at path: " << pathToTexture << std::endl;
			isInitialized = false;
		}
		freeSTBIImageData(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

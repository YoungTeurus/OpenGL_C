#include "Texture.h"

#include <glad/glad.h>
#include "../utility/FilePaths.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

void Texture::setSTBIFilpState(const bool flipState)
{
	stbi_set_flip_vertically_on_load(flipState);
}

void Texture::loadTextureAndSetFields(const std::string& fileName, const std::string& directory)
{
	this->fileName = fileName;
	this->directory = directory;
	this->type = type;

	const std::string pathToFile = directory + FilePaths::getSplitter() + fileName;

    glGenTextures(1, &id);

    int width, height, nrComponents;
    unsigned char* data = loadImageData(pathToFile, &width, &height, &nrComponents);
    if (data)
    {
        GLenum format;
        switch (nrComponents) {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
        default:
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	    isInitialized = true;
    }
    else
    {
        std::cout << "Texture failed to load at path: " << pathToFile << std::endl;
    }
    freeSTBIImageData(data);
}

unsigned char* Texture::loadImageData(std::string pathToFile, int* width, int* height, int* nrComponents)
{
	return stbi_load(pathToFile.c_str(), width, height, nrComponents, 0);
}

void Texture::freeSTBIImageData(unsigned char* data)
{
	stbi_image_free(data);
}

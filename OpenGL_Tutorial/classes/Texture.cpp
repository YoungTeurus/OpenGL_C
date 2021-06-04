#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

Texture::Texture(TextureType textureType)
	:type(textureType), textureID(-1), filename("")
{}

Texture::Texture(const char* pathToTexture, TextureType textureType)
	:type(textureType)
{
	loadFromFileAndSetTextureID(pathToTexture);
}

Texture::Texture(const char* filename, const std::string& directory, TextureType textureType)
	:type(textureType)
{
	this->filename = std::string(filename);
	std::string pathToTexture = "D:/Coding/C++/OpenGL_Tutorial/OpenGL_Tutorial/" + directory + '/' + this->filename;
	
	loadFromFileAndSetTextureID(pathToTexture.c_str());
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
}

void Texture::loadFromFileAndSetTextureID(const char* pathToTexture)
{
	if (textureID != -1) {
		throw std::runtime_error("Texture::loadFromFileAndSetTextureID: there was an attempt to reload alreeady loaded texture!");
	}

	int width, height, nrChannels;
	
	unsigned char textureData[1];
	
	height = width = nrChannels = 1;
	// try{
	// 	textureData = stbi_load(pathToTexture, &width, &height, &nrChannels, 0);
	// } catch(const std::exception e)
	// {
	// 	stbi_image_free(textureData);
	// 	return;
	// }

	if (!textureData) {
		stbi_image_free(textureData);
		throw std::runtime_error((std::string("Texture::Texture: failed loading texture at path: ") + pathToTexture).c_str());
	}

	GLenum textureRGBMode;
	switch (nrChannels)
	{
	case 1:
		textureRGBMode = GL_RED;
		break;
	case 3:
		textureRGBMode = GL_RGB;
		break;
	case 4:
	default:
		textureRGBMode = GL_RGBA;
		break;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);  // ��� �������� ��� GL_TEXTURE_2D ����� ����������� ��� ������ ���������.
	
	// ��������� ���������� ��������� (������������) ��������� ��� ����� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ��������� ���������� ������������ ��������: �������� ������������ (� �������������� mipmap)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// ��������� ���������:
	glTexImage2D(
		GL_TEXTURE_2D,		// ��� ��������
		0,					// ������� ���-�������
		GL_RGB,	// ������ �������� ��������
		width,				// ������� �����������
		height,				// - ^ - ^ - ^ -
		0,					// "Legacy stuff"
		(int)textureRGBMode,	// ������ ��������� �����������
		GL_UNSIGNED_BYTE,	// ������ ������ ��������� �����������
		textureData		// ��������� �� ������ � ������������
	);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(textureData); // ������������ ������
}

unsigned Texture::ID() const
{
	return this->textureID;
}

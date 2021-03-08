#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char* texturePath)
{
	int width, height, nrChannels;
	unsigned char* texture = stbi_load(texturePath, &width, &height, &nrChannels, 0);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);  // ��� �������� ��� GL_TEXTURE_2D ����� ����������� ��� ������ ���������.
	// ��������� ���������� ��������� (������������) ��������� ��� ����� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ��������� ���������� ������������ ��������: �������� ������������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (texture) {
		// ��������� ���������:
		glTexImage2D(
			GL_TEXTURE_2D,		// ��� ��������
			0,					// ������� ���-�������
			GL_RGB,				// ������ �������� ��������
			width,				// ������� �����������
			height,				// - ^ - ^ - ^ -
			0,					// "Legacy stuff"
			GL_RGB,				// ������ ��������� �����������
			GL_UNSIGNED_BYTE,	// ������ ������ ��������� �����������
			texture		// ��������� �� ������ � ������������
		);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(texture); // ������������ ������
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
}

void Texture::use()
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}

unsigned Texture::ID()
{
	return this->textureID;
}

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

Texture::Texture(TextureType textureType)
	:type(textureType), textureID(-1), filename("")
{}

Texture::Texture(const char* pathToTexture, TextureType textureType)
	:type(textureType), filename("")
{
	loadFromFileAndSetTextureID(pathToTexture);
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
	// stbi_set_flip_vertically_on_load(true);
	unsigned char* texture = stbi_load(pathToTexture, &width, &height, &nrChannels, 0);

	if (!texture) {
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
		textureRGBMode = GL_RGBA;
		break;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);  // Все операции над GL_TEXTURE_2D будут происходить над данной текстурой.
	// Настройка параметров замещения (растягивания) текустуры для обоих осей
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Настройка параметров интерполяции текстуры: линейная интерполяция (с использованием mipmap)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (texture) {
		// Генерация текустуры:
		glTexImage2D(
			GL_TEXTURE_2D,		// Тип текстуры
			0,					// Уровень мип-мапинга
			GL_RGB,	// Формат хранимой текстуры
			width,				// Размеры изображения
			height,				// - ^ - ^ - ^ -
			0,					// "Legacy stuff"
			(int)textureRGBMode,	// Формат исходного изображения
			GL_UNSIGNED_BYTE,	// Формат данных исходного изображения
			texture		// Указатель на массив с изображением
		);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(texture); // Освобождение памяти
}

unsigned Texture::ID() const
{
	return this->textureID;
}

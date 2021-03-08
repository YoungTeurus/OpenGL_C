#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char* texturePath)
{
	int width, height, nrChannels;
	unsigned char* texture = stbi_load(texturePath, &width, &height, &nrChannels, 0);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);  // Все операции над GL_TEXTURE_2D будут происходить над данной текстурой.
	// Настройка параметров замещения (растягивания) текустуры для обоих осей
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Настройка параметров интерполяции текстуры: линейная интерполяция
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (texture) {
		// Генерация текустуры:
		glTexImage2D(
			GL_TEXTURE_2D,		// Тип текстуры
			0,					// Уровень мип-мапинга
			GL_RGB,				// Формат хранимой текстуры
			width,				// Размеры изображения
			height,				// - ^ - ^ - ^ -
			0,					// "Legacy stuff"
			GL_RGB,				// Формат исходного изображения
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

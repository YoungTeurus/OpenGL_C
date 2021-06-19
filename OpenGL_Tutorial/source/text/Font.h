#pragma once
#include <string>
#include <map>
#include <glm/glm.hpp>

#include "../textures/Texture.h"

#include "ft2build.h"
#include FT_FREETYPE_H


struct Character
{
	Texture texture;
	glm::ivec2 size;		// Размер глифа
	glm::ivec2 bearing;		// Отступ от baseline-а вправо-вверх верхнего левого угла глифа
	unsigned advance;		// Расстояние до следующего глифа в 1/64 пикселя
};

class Font
{
private:
	FT_Library library;
	FT_Face face;
	
	std::string name;

	std::map<char, Character> characters;

	void initializeLibrary();
	void loadCharacters();
public:
	Font(const std::string& name, unsigned fontSize = 48);

	Character getCharacter(const char& c) const
	{
		return characters.at(c);
	}
};

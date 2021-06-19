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
	glm::ivec2 size;		// ������ �����
	glm::ivec2 bearing;		// ������ �� baseline-� ������-����� �������� ������ ���� �����
	unsigned advance;		// ���������� �� ���������� ����� � 1/64 �������
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

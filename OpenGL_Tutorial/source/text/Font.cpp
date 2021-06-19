#include "Font.h"


#include "../textures/FontTexture.h"
#include "../utility/FilePaths.h"

void Font::initializeLibrary()
{
	FT_Init_FreeType(&library);
}

void Font::loadCharacters()
{
	for (unsigned char c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			throw std::exception("Font::loadCharacters::FREETYTPE: Failed to load Glyph");
		}

		FontTexture fontTexture;
		fontTexture.loadTextureAndSetFields(face);

		Character character = {
			fontTexture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		characters.insert(std::pair<char, Character>(c, character));
	}
}

Font::Font(const std::string& name, unsigned fontSize)
{
	this->name = name;
	initializeLibrary();

	if(FT_New_Face(library, FilePaths::getPathToFont(name + ".ttf").c_str(), 0, &face))
	{
		throw std::exception("Font::Font::FREETYPE: Failed to load font");
	}

	FT_Set_Pixel_Sizes(face, 0, fontSize);

	loadCharacters();

	FT_Done_Face(face);
	FT_Done_FreeType(library);
}

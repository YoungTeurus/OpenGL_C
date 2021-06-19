#pragma once
#include <freetype/freetype.h>
#include <glad/glad.h>

#include "Texture.h"

class FontTexture : public Texture
{
public:
	void loadTextureAndSetFields(FT_Face face)
	{
		type = TextureType::FONT_GLYPH;

		// generate texture
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		
	    glGenTextures(1, &id);
	    glBindTexture(GL_TEXTURE_2D, id);
	    glTexImage2D(
	        GL_TEXTURE_2D,
	        0,
	        GL_RED,
	        face->glyph->bitmap.width,
	        face->glyph->bitmap.rows,
	        0,
	        GL_RED,
	        GL_UNSIGNED_BYTE,
	        face->glyph->bitmap.buffer
	    );
	    // set texture options
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		isInitialized = true;
	}
};

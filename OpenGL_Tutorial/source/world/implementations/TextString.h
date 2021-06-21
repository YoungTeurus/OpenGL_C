#pragma once
#include "../../model/VAOBuilder.h"
#include "../../model/VOsAndIndices.h"
#include "../../renderer/Renderer.h"
#include "../../text/Font.h"
#include "../../utility/TemplateString.h"
#include "../interfaces/DrawableObject.h"

class TextString : public DrawableObject
{
private:
	VOsAndIndices* fontVOsAndIndices = VAOBuilder::getInstance()->getFont();
	Font* font;
	TemplateString templateString;
	string text;

	glm::vec2 position;
	float scale;
	glm::vec3 color;
public:
	TextString(Font* font, const std::string& initialText, const glm::vec2& position, const float& scale, const glm::vec3& color)
		:DrawableObject("font"),
		 font(font), templateString(initialText), position(position),scale(scale), color(color)
	{
		text = templateString.getString();
	}

	void setKeysValues(const vector<pair<string, string>>& keysValues)
	{
		text = templateString.replaceKeys(keysValues);
	}

	void drawAction(Renderer* renderer) override
	{
		// activate corresponding render state	
		shader->use();
		shader->setFloatMat4("projection", renderer->getOrthoProjection());
		shader->setFloatVec3("uTextColor", color);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(fontVOsAndIndices->vao);

		float x = position.x, y = position.y;

		// iterate through all characters
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
		    Character ch = font->getCharacter(*c);

		    float xpos = x + ch.bearing.x * scale;
		    float ypos = y - (ch.size.y - ch.bearing.y) * scale;

		    float w = ch.size.x * scale;
		    float h = ch.size.y * scale;
		    // update VBO for each character
		    float vertices[4][4] = {
		        { xpos,     ypos + h,   0.0f, 0.0f },            
		        { xpos,     ypos,       0.0f, 1.0f },
		        { xpos + w, ypos,       1.0f, 1.0f },
		        { xpos + w, ypos + h,   1.0f, 0.0f }           
		    };
		    // render glyph texture over quad
		    glBindTexture(GL_TEXTURE_2D, ch.texture.getId());
		    // update content of VBO memory
		    glBindBuffer(GL_ARRAY_BUFFER, fontVOsAndIndices->vbo);
		    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Загружаем VBO новыми данными.
		    glBindBuffer(GL_ARRAY_BUFFER, 0);
		    // render quad
		    glBindVertexArray(fontVOsAndIndices->vao);
			glDrawElements(GL_TRIANGLES, fontVOsAndIndices->indices.size(), GL_UNSIGNED_INT, 0);
		    // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		    x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	glm::vec3 getColor() const
	{
		return color;
	}

	void setColor(const glm::vec3& color)
	{
		this->color = color;
	}

	float getScale() const
	{
		return scale;
	}

	void setScale(float scale)
	{
		this->scale = scale;
	}

	void setFont(Font* font)
	{
		this->font = font;
	}

	std::string getText() const
	{
		return text;
	}

	void setText(const std::string& text)
	{
		this->text = text;
	}

	glm::vec2 getPosition() const
	{
		return position;
	}

	void setPosition(const glm::vec2& position)
	{
		this->position = position;
	}
};

#pragma once
#include "../../model/VAOBuilder.h"
#include "../../model/VOsAndIndices.h"
#include "../interfaces/DrawableObject.h"

class LineStrip : public DrawableObject
{
	glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 startPoint;
	glm::vec3 endPoint;
	VOsAndIndices* lineStripVOsAndIndices = VAOBuilder::getInstance()->getLineStrip();

	void drawAction() override
	{
		shader->use();
		shader->setFloatMat4("projectionAndView", getAttachedRenderer()->getPV());
		shader->setFloatVec3("uColor", color);

		float vertices[2][3] = {
		        { startPoint.x, startPoint.y, startPoint.z },            
		        { endPoint.x, endPoint.y, endPoint.z }
		};

		glBindBuffer(GL_ARRAY_BUFFER, lineStripVOsAndIndices->vbo);
	    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Загружаем VBO новыми данными.
	    glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glBindVertexArray(lineStripVOsAndIndices->vao);
		glDrawElements(GL_LINES, lineStripVOsAndIndices->indices.size(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}

public:
	LineStrip(Renderer* renderer, const glm::vec3& startPoint, const glm::vec3& endPoint)
		:DrawableObject(renderer, "lineStrip"), startPoint(startPoint), endPoint(endPoint)
	{
	}


	glm::vec3 getColor() const
	{
		return color;
	}

	void setColor(const glm::vec3& color)
	{
		this->color = color;
	}

	glm::vec3 getStartPoint() const
	{
		return startPoint;
	}

	void setStartPoint(const glm::vec3& startPoint)
	{
		this->startPoint = startPoint;
	}

	glm::vec3 getEndPoint() const
	{
		return endPoint;
	}

	void setEndPoint(const glm::vec3& endPoint)
	{
		this->endPoint = endPoint;
	}
};

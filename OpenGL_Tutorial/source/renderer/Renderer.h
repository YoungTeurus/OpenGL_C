#pragma once
#include <glm/glm.hpp>

class Renderer
{
private:
	static Renderer* instance;

	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 pv;

	Renderer()
	{
	}
public:
	static Renderer* getInstance()
	{
		if (instance == nullptr)
		{
			instance = new Renderer();
		}
		return instance;
	}

	void setViewAndProjection(const glm::mat4& view, const glm::mat4& projection)
	{
		this->view = view;
		this->projection = projection;
		this->pv = projection * view;
	}

	glm::mat4 getPV()
	{
		return pv;
	}
};

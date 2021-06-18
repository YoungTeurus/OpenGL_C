#pragma once
#include <vector>
#include <glm/glm.hpp>


#include "../camera/Camera.h"
#include "../light/BaseLight.h"

class Renderer
{
private:
	static Renderer* instance;

	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 pv;

	std::vector<BaseLight*> lights;

	Camera mainCamera;

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

	void addLight(BaseLight* light)
	{
		lights.push_back(light);
	}

	std::vector<BaseLight*> getLights() const
	{
		return lights;
	}

	Camera getMainCamera() const
	{
		return mainCamera;
	}

	glm::mat4 getPV()
	{
		return pv;
	}
};

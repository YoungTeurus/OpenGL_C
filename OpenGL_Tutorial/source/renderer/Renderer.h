#pragma once
#include <vector>
#include <glm/glm.hpp>


#include "../camera/Camera.h"
#include "../light/BaseLight.h"
#include "../textures/TexturesLoader.h"

class Renderer
{
private:
	static Renderer* instance;

	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 pv;

	std::vector<BaseLight*> lights;

	Camera *mainCamera;

	Renderer(const glm::ivec2& viewSize = {0, 0})
	{
		mainCamera = new Camera(viewSize);
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

	Camera* getMainCamera() const
	{
		return mainCamera;
	}

	void updateViewAndProjection()
	{
		const glm::mat4 view = mainCamera->getViewMatrix();
 		const glm::mat4 projection = mainCamera->getProjectionMatrix();
	 
 		setViewAndProjection(view, projection);
	}

	glm::mat4 getPV()
	{
		return pv;
	}

	glm::mat4 getView() const
	{
		return view;
	}

	glm::mat4 getProjection() const
	{
		return projection;
	}

	void setScreenWidthAndHeight(const int& width, const int& height)
	{
		mainCamera->setViewSize({width, height});
	}

	glm::mat4 getOrthoProjection() const
	{
		return mainCamera->getOrthoProjection();
	}
};

#pragma once
#include <vector>


#include "../world/DrawableObject.h"
#include "../world/Skybox.h"
#include "../world/WorldObject.h"

class Scene
{
public:
	std::vector<WorldObject*> allObjects;
	std::vector<DrawableObject*> drawableObjects;
	Skybox* skybox = nullptr;
	glm::vec4 backgroundColor = {0.25f, 0.25f, 0.25f, 1.f};

	void draw(Renderer* renderer)
	{
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		for (auto * drawableObject : drawableObjects)
		{
			drawableObject->draw(renderer);
		}
		if (skybox != nullptr)
		{
			skybox->draw(renderer);
		}
	}

	void addObject(WorldObject* worldObject)
	{
		allObjects.push_back(worldObject);
	}

	void addDrawableObject(DrawableObject* drawableObject)
	{
		allObjects.push_back(drawableObject);
		drawableObjects.push_back(drawableObject);
	}

	void addSkybox(Skybox* skybox)
	{
		allObjects.push_back(skybox);
		this->skybox = skybox;
	}
};

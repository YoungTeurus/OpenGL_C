#pragma once
#include <vector>



#include "../world/CollidableObject.h"
#include "../world/DrawableObject.h"
#include "../world/Skybox.h"
#include "../world/WorldObject.h"

class Scene
{
public:
	std::vector<WorldObject*> allObjects;
	std::vector<DrawableObject*> drawableObjects;
	std::vector<CollidableObject*> collidableObjects;
	std::vector<CollidableObject*> dynamicCollidableObjects;
	
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

	void checkCollisions()
	{
		for (auto *dynamicCollidableObject : dynamicCollidableObjects)
		{
			for(auto *other : collidableObjects)
			{
				if (dynamicCollidableObject == other)
				{
					continue;
				}
				dynamicCollidableObject->checkCollisionAndCorrectPositionIfNeeded(other);
			}
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

	void addCollidableObject(CollidableObject* collidableObject)
	{
		allObjects.push_back(collidableObject);
		collidableObjects.push_back(collidableObject);
	}

	void addDynamicCollidableObject(CollidableObject* collidableObject)
	{
		allObjects.push_back(collidableObject);
		collidableObjects.push_back(collidableObject);
		dynamicCollidableObjects.push_back(collidableObject);
	}

	void addSkybox(Skybox* skybox)
	{
		allObjects.push_back((PositionedWorldObject*)skybox);
		this->skybox = skybox;
	}
};

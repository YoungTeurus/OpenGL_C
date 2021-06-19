#pragma once
#include <vector>



#include "../world/CollidableDrawableObject.h"
#include "../world/CollidableObject.h"
#include "../world/DrawableObject.h"
#include "../world/Skybox.h"
#include "../world/WorldObject.h"

class Scene
{
private:
	bool drawColliders = false;
public:
	std::vector<WorldObject*> allObjects;
	std::vector<DrawableObject*> drawableObjects;
	std::vector<CollidableObject*> collidableObjects;
	std::vector<CollidableDrawableObject*> collidableDrawableObjects;
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

	void toggleCollidersDrawing()
	{
		drawColliders = !drawColliders;
		for (auto *collidableDrawableObject : collidableDrawableObjects)
		{
			collidableDrawableObject->setColliderDrawing(drawColliders);
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

	void addCollidableObject(CollidableDrawableObject* collidableObject)
	{
		allObjects.push_back((PositionedWorldObject*)collidableObject);
		drawableObjects.push_back(collidableObject);
		collidableObjects.push_back(collidableObject);
		collidableDrawableObjects.push_back(collidableObject);
	}

	void addDynamicCollidableObject(CollidableDrawableObject* collidableObject)
	{
		allObjects.push_back((PositionedWorldObject*)collidableObject);
		drawableObjects.push_back(collidableObject);
		collidableObjects.push_back(collidableObject);
		collidableDrawableObjects.push_back(collidableObject);
		dynamicCollidableObjects.push_back(collidableObject);
	}

	void addSkybox(Skybox* skybox)
	{
		allObjects.push_back((PositionedWorldObject*)skybox);
		this->skybox = skybox;
	}
};

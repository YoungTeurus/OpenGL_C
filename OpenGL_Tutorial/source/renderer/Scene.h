#pragma once
#include <vector>



#include "../world/implementations/Skybox.h"
#include "../world/implementations/Tank.h"
#include "../world/interfaces/CollidableDrawableObject.h"
#include "../world/interfaces/CollidableObject.h"
#include "../world/interfaces/DrawableObject.h"
#include "../world/interfaces/DrawableUpdatableObject.h"
#include "../world/interfaces/UpdatableObject.h"
#include "../world/interfaces/WorldObject.h"


class Scene
{
private:
	bool drawColliders = false;
public:
	std::vector<WorldObject*> allObjects;
	std::vector<DrawableObject*> drawableObjects;
	std::vector<UpdatableObject*> updatableObjects;
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

	void update(const float& currentTime)
	{
		checkCollisions();
		for(auto *updatableObject : updatableObjects)
		{
			updatableObject->update(currentTime);
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
		addObject(drawableObject);
		drawableObjects.push_back(drawableObject);
	}

	void addDrawableUpdatableObject(DrawableUpdatableObject* drawableUpdatableObject)
	{
		addDrawableObject((DrawableObject*)drawableUpdatableObject);
		updatableObjects.push_back(drawableUpdatableObject);
	}

	void addCollidableDrawableObject(CollidableDrawableObject* collidableDrawableObject)
	{
		addDrawableObject((DrawableObject*)collidableDrawableObject);
		collidableObjects.push_back(collidableDrawableObject);
		collidableDrawableObjects.push_back(collidableDrawableObject);
	}

	void addDynamicCollidableDrawableObjectObject(CollidableDrawableObject* dynamicCollidableDrawableObject)
	{
		addCollidableDrawableObject(dynamicCollidableDrawableObject);
		dynamicCollidableObjects.push_back(dynamicCollidableDrawableObject);
	}

	void addTank(Tank* tankObject)
	{
		addDynamicCollidableDrawableObjectObject(tankObject);
		updatableObjects.push_back(tankObject);
	}

	void addSkybox(Skybox* skybox)
	{
		allObjects.push_back((PositionedObject*)skybox);
		this->skybox = skybox;
	}
};

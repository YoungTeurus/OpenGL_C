#pragma once
#include <vector>



#include "../world/implementations/Skybox.h"
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
		allObjects.push_back(drawableObject);
		drawableObjects.push_back(drawableObject);
	}

	void addDrawableUpdatableObject(DrawableUpdatableObject* drawableUpdatableObject)
	{
		allObjects.push_back((DrawableObject*)drawableUpdatableObject);
		drawableObjects.push_back(drawableUpdatableObject);
		updatableObjects.push_back(drawableUpdatableObject);
	}

	void addCollidableObject(CollidableDrawableObject* collidableObject)
	{
		allObjects.push_back((PositionedObject*)collidableObject);
		drawableObjects.push_back(collidableObject);
		updatableObjects.push_back(collidableObject);
		collidableObjects.push_back(collidableObject);
		collidableDrawableObjects.push_back(collidableObject);
	}

	void addDynamicCollidableObject(CollidableDrawableObject* collidableObject)
	{
		allObjects.push_back((PositionedObject*)collidableObject);
		drawableObjects.push_back(collidableObject);
		updatableObjects.push_back(collidableObject);
		collidableObjects.push_back(collidableObject);
		collidableDrawableObjects.push_back(collidableObject);
		dynamicCollidableObjects.push_back(collidableObject);
	}

	void addSkybox(Skybox* skybox)
	{
		allObjects.push_back((PositionedObject*)skybox);
		this->skybox = skybox;
	}
};

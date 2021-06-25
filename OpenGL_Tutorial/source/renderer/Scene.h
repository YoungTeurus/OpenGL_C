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
	std::vector<TankBullet*> bullets;
	
	Skybox* skybox = nullptr;
	glm::vec4 backgroundColor = {0.25f, 0.25f, 0.25f, 1.f};

	void draw()
	{
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		for (auto * drawableObject : drawableObjects)
		{
			drawableObject->draw();
		}
		if (skybox != nullptr)
		{
			skybox->draw();
		}
	}

	void update(const float& deltaTime)
	{
		removeDeadObjects();
		checkCollisions();
		for(auto *updatableObject : updatableObjects)
		{
			updatableObject->update(deltaTime);
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

	void addBullet(TankBullet* bulletObject)
	{
		addDynamicCollidableDrawableObjectObject(bulletObject);
		updatableObjects.push_back(bulletObject);
		bullets.push_back(bulletObject);
	}

	void removeDeadObjects()
	{
		// remove_if(allObjects.begin(), allObjects.end(), WorldObject::isReadyToBeRemoved);
		// remove_if(drawableObjects.begin(), drawableObjects.end(), DrawableObject::isReadyToBeRemoved);
		// remove_if(updatableObjects.begin(), updatableObjects.end(), UpdatableObject::isReadyToBeRemoved);
		// remove_if(collidableObjects.begin(), collidableObjects.end(), CollidableObject::isReadyToBeRemoved);
		// remove_if(collidableDrawableObjects.begin(), collidableDrawableObjects.end(), CollidableDrawableObject::isReadyToBeRemoved);
		// remove_if(dynamicCollidableObjects.begin(), dynamicCollidableObjects.end(), CollidableObject::isReadyToBeRemoved);
		// remove_if(bullets.begin(), bullets.end(), TankBullet::isReadyToBeRemoved);
	}

	void addSkybox(Skybox* skybox)
	{
		allObjects.push_back((PositionedObject*)skybox);
		this->skybox = skybox;
	}
};

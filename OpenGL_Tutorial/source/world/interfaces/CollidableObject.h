#pragma once
#include <algorithm>
#include <glm/glm.hpp>

#include "PositionedObject.h"
#include "../../model/ModelTransformations.h"

struct CollizionResolver
{
	glm::vec3 pushOutVector;
	float pushOutOffset;
};

struct OffsetAndSize
{
	glm::vec3 offset;
	glm::vec3 size;
};

using namespace std;
class ColliderCube
{
public:

	// Координаты ...2 > ...1
	float x1, y1, z1, x2, y2, z2;

	ColliderCube(const glm::vec3& objectPosition, const OffsetAndSize& offsetAndSize)
		:ColliderCube(objectPosition, offsetAndSize.offset, offsetAndSize.size)
	{
	}

	ColliderCube(const glm::vec3& objectPosition, const glm::vec3& offset, const glm::vec3& size)
	{
		x1 = objectPosition.x + offset.x;
		y1 = objectPosition.y + offset.y;
		z1 = objectPosition.z + offset.z;

		x2 = x1 + size.x;
		y2 = y1 + size.y;
		z2 = z1 + size.z;
	}

	ColliderCube(float x1, float y1, float z1, float x2, float y2, float z2)
		: x1(min(x1, x2)),
		  y1(min(y1, y2)),
		  z1(min(z1, z2)),
		  x2(max(x1, x2)),
		  y2(max(y1, y2)),
		  z2(max(z1, z2))
	{
	}

	bool hasCollision(const ColliderCube& other) const
	{
		//    (x1,y1)
		//     *--------------*
		//     |			  |
		//	   |			  |
		//	   |	   *------|-----------*
		//	   |	   |      |			  |
		//	   |	   |      |			  |
		//	   *-------|------*			  |
		//			   |    (x2, y2)      |
		//			   |				  |
		//			   |				  |
		//			   |				  |
		//			   |				  |
		//			   *------------------*
		//

		bool xIntersects = false, yIntersects = false, zIntersects = false;

		xIntersects = (x1 <= other.x1 && x2 >= other.x1) || (x1 <= other.x2 && x2 >= other.x2);
		if(!xIntersects)
		{
			return false;
		}
		yIntersects = (y1 <= other.y1 && y2 >= other.y1) || (y1 <= other.y2 && y2 >= other.y2);
		if(!yIntersects)
		{
			return false;
		}
		zIntersects = (z1 <= other.z1 && z2 >= other.z1) || (z1 <= other.z2 && z2 >= other.z2);
		if(!zIntersects)
		{
			return false;
		}
		
		return true;
	}

	CollizionResolver getCollizionResolver(const ColliderCube& other) const
	{
		glm::vec3 pushOutVector;
		float pushOutOffset;

		const glm::vec3 selfSize = getSize();
		const glm::vec3 otherSize = other.getSize();
		
		const glm::vec3 centerVector = getCenterVector(other);
		const float centerVectorLength = glm::length(centerVector);
		
		const float xOffset = centerVector.x;
		const float yOffset = centerVector.y;
		const float zOffset = centerVector.z;

		const bool isXPositiveOrZero = xOffset >= 0.0f;
		const bool isYPositiveOrZero = yOffset >= 0.0f;
		const bool isZPositiveOrZero = zOffset >= 0.0f;

		const float xAbs = abs(xOffset);
		const float yAbs = abs(yOffset);
		const float zAbs = abs(zOffset);

		if (xAbs >= yAbs && xAbs >= zAbs)
		{
			pushOutVector = glm::vec3( isXPositiveOrZero ? 1.0f : -1.0f , 0.0f, 0.0f);

			const float xMinimalOffset = selfSize.x / 2 + otherSize.x / 2;
			pushOutOffset = xMinimalOffset - abs(xOffset);
		} else if (yAbs >= xAbs && yAbs >= zAbs)
		{
			pushOutVector = glm::vec3( 0.0f, isYPositiveOrZero ? 1.0f : -1.0f, 0.0f);

			const float yMinimalOffset = selfSize.y / 2 + otherSize.y / 2;
			pushOutOffset = yMinimalOffset - abs(yOffset);
		} else // if (zAbs >= xAbs && zAbs >= yAbs)
		{
			pushOutVector = glm::vec3( 0.0f, 0.0f, isZPositiveOrZero ? 1.0f : -1.0f);

			const float zMinimalOffset = selfSize.z / 2 + otherSize.z / 2;
			pushOutOffset = zMinimalOffset - abs(zOffset);
		}

		return CollizionResolver{pushOutVector, pushOutOffset};
	}

	glm::vec3 getCenterVector(const ColliderCube& other) const
	{
		const glm::vec3 center = getCenter();
		const glm::vec3 otherCenter = other.getCenter();
		return center - otherCenter;
	}

	glm::vec3 getCenter() const
	{
		glm::vec3 size = getSize();
		return glm::vec3(size.x / 2 + x1, size.y / 2 + y1, size.z / 2 + z1);
	}
	
	glm::vec3 getSize() const
	{
		return glm::vec3( x2 - x1, y2 - y1, z2 - z1 );
	}

	void setPosition(const glm::vec3& position)
	{
		const glm::vec3 size = getSize();
		x1 = position.x;
		y1 = position.y;
		z1 = position.z;
		x2 = x1 + size.x;
		y2 = y1 + size.y;
		z2 = z1 + size.z;
	}
	
	void offsetPosition(const glm::vec3& offset)
	{
		x1 += offset.x;
		x2 += offset.x;
		y1 += offset.y;
		y2 += offset.y;
		z1 += offset.z;
		z2 += offset.z;
	}

	glm::vec3 getPosition() const
	{
		return glm::vec3(x1, y1, z1);
	}

	void scale(const glm::vec3& scale)
	{
		glm::vec3 size = getSize();
		size *= scale;
		x2 = x1 + size.x;
		y2 = y2 + size.y;
		z2 = z2 + size.z;
	}

	string toString()
	{
		return string("{") + 
			"(" + to_string(x1) + "," + to_string(y1) + "," + to_string(z1) + ")" + 
			"," +
			"(" + to_string(x2) + "," + to_string(y2) + "," + to_string(z2) + ")" +
			"}";
	}
};

class CollidableObject : public PositionedObject
{
	// TODO: вернуть модификатор доступа private.
public:
	ColliderCube colliderCube;
	OffsetAndSize colliderOffsetAndSize;

	CollizionResolver getCollizionResolver(CollidableObject *other)
	{
		return colliderCube.getCollizionResolver(other->colliderCube);
	}
protected:
	OffsetAndSize getColliderOffsetAndSize() const
		{
			return colliderOffsetAndSize;
		}
public:

	CollidableObject(const ModelTransformations& transformations, const OffsetAndSize& colliderOffsetAndSize)
		:PositionedObject(transformations), colliderCube(ColliderCube{transformations.position, colliderOffsetAndSize}),
		 colliderOffsetAndSize(colliderOffsetAndSize)
	{
	}

	void checkCollisionAndCorrectPositionIfNeeded(CollidableObject *other)
	{
		if (hasCollision(other))
		{
			CollizionResolver collizionResolver = getCollizionResolver(other);
			offsetPosition( collizionResolver.pushOutVector * collizionResolver.pushOutOffset );
		}
	}
	
	bool hasCollision(CollidableObject *other)
	{
		return colliderCube.hasCollision(other->colliderCube);
	}

	void setScale(const glm::vec3& scale) override
	{
		PositionedObject::setScale(scale);
		colliderCube.scale(scale);
	}

	void setPosition(glm::vec3 pos) override
	{
		PositionedObject::setPosition(pos);
		colliderCube.setPosition(pos);
		colliderCube.offsetPosition(colliderOffsetAndSize.offset);
	}

	void offsetPosition(const glm::vec3& offset) override
	{
		PositionedObject::offsetPosition(offset);
		colliderCube.offsetPosition(offset);
	}

	virtual std::string toString() override
	{
		return PositionedObject::toString() + " collCube:" + colliderCube.toString(); 
	}
};

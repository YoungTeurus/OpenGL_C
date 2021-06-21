#pragma once
#include <vector>
#include <map>

enum class LevelObject
{
	NOTHING = 0,
	BRICK_WALL = 1,
	STONE_WALL = 2,
	PLAYER_SPAWN = 7,
	ENEMY_SPAWN = 8,
	PLAYER_BASE = 9,

	UNKNOWN = 999,
};

std::map<unsigned, LevelObject> intToLevelObject = {
	{0, LevelObject::NOTHING},
	{1, LevelObject::BRICK_WALL},
	{2, LevelObject::STONE_WALL},
	{7, LevelObject::PLAYER_SPAWN},
	{8, LevelObject::ENEMY_SPAWN},
	{9, LevelObject::PLAYER_BASE},
};

inline LevelObject getLevelObjectFromNumber(const unsigned& number)
{
	try{
		return intToLevelObject.at(number);
	} catch (std::out_of_range&)
	{
		return LevelObject::UNKNOWN;
	}
}

class Level
{
private:
	float gridSize = 2.0f;
	unsigned width;
	unsigned height;

	LevelObject** levelObjects;

	void clearLevelObjects()
	{
		if (levelObjects == nullptr)
		{
			return;
		}
		for(unsigned w = 0; w < width; w++)
		{
			delete[] levelObjects[w];
		}
		delete[] levelObjects;
		levelObjects = nullptr;
	}

	void clearAndCreateLevelObjectsGrid()
	{
		clearLevelObjects();
		
		levelObjects = new LevelObject*[width];
		for(unsigned w = 0; w < width; w++)
		{
			levelObjects[w] = new LevelObject[height];
			for(unsigned h = 0; h < height; h++)
			{
				levelObjects[w][h] = LevelObject::NOTHING;
			}
		}
	}

	void populateLevelObjectsGridFromArray(const LevelObject** objects)
	{
		for(unsigned w = 0; w < width; w++)
		{
			for(unsigned h = 0; h < height; h++)
			{
				levelObjects[w][h] = objects[w][h];
			}
		}
	}

	void populateLevelObjectsGridFromArray(const std::vector<LevelObject>& objects)
	{
		for(unsigned w = 0; w < width; w++)
		{
			for(unsigned h = 0; h < height; h++)
			{
				levelObjects[w][h] = objects[w * width + h];
			}
		}
	}

public:
	Level(const unsigned& width, const unsigned& height, const LevelObject** objects)
		:Level(width, height)
	{
		populateLevelObjectsGridFromArray(objects);
	}
	
	Level(const unsigned& width, const unsigned& height, const std::vector<LevelObject>& objects)
		:Level(width, height)
	{
		populateLevelObjectsGridFromArray(objects);
	}
	
	Level(const unsigned& width, const unsigned& height)
		:width(width), height(height)
	{
		clearAndCreateLevelObjectsGrid();
	}
};

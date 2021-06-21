#pragma once
#include <string>
#include <fstream>

#include "Level.h"
#include "../utility/FilePaths.h"

using namespace std;

class LevelLoader
{
public:
	static Level* loadFromFile(const string& fileName, const unsigned& levelWidth, const unsigned& levelHeight)
	{
		return loadFromFile(fileName, FilePaths::getPathToLevelsFolder(), levelWidth, levelHeight);
	}
	
	static Level* loadFromFile(const string& fileName, const string& folder, const unsigned& levelWidth, const unsigned& levelHeight)
	{
		string pathToFile = folder + FilePaths::getSplitter() + fileName;		
		ifstream file(pathToFile);
		if(!file.is_open())
		{
			throw exception(("LevelLoader::loadFromFile: file '" + pathToFile + "' can't be opened!").c_str());
		}

		vector<LevelObject> loadedObjects;
		string element;
		while(file >> element)
		{
			unsigned number;
			try{
				number = stoul(element);
			} catch (std::invalid_argument&)
			{
				file.close();
				throw exception(("LevelLoader::loadFromFile: can't resolve symbol '" + element + "' as unsigned int!").c_str());
			}
			const LevelObject object = getLevelObjectFromNumber(number);
			if (object == LevelObject::UNKNOWN)
			{
				file.close();
				throw exception(("LevelLoader::loadFromFile: can't resolve symbol '" + element + "' as LevelObject!").c_str());
			}
			loadedObjects.push_back(object);
		}
		
		file.close();

		return new Level(levelWidth, levelHeight, loadedObjects);
	}
};

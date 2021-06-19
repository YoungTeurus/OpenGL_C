#pragma once
#include <map>
#include <string>

#include "Font.h"

class FontLoader{
private:
	static FontLoader* instance;

	std::map<std::string, Font*>* loadedFonts;
	
	FontLoader()
	{
		loadedFonts = new std::map<std::string, Font*>();
	}
public:
	static FontLoader* getInstance()
	{
		if (instance == nullptr)
		{
			instance = new FontLoader();
		}
		return instance;
	}

	Font* load(const std::string& name, const unsigned& fontSize){
		const std::string key = name + "_" + std::to_string(fontSize);

		Font* newFont = new Font(name, fontSize);

		loadedFonts->insert(std::pair<std::string, Font*>(key, newFont));
		
		return newFont;
	}

	Font* get(const std::string& name, const unsigned& fontSize) const
	{
		const std::string key = name + "_" + std::to_string(fontSize);
		const auto it = loadedFonts->find(key);
		if(it != loadedFonts->end())
		{
			return it->second;
		}
		return nullptr;
	}

	Font* getOrLoad(const std::string& name, const unsigned& fontSize)
	{
		const std::string key = name + "_" + std::to_string(fontSize);
		const auto it = loadedFonts->find(key);
		if(it != loadedFonts->end())
		{
			return it->second;
		}
		return load(name, fontSize);
	}
};

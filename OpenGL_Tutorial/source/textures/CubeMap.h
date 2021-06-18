#pragma once

#include <vector>

#include "Texture.h"

class CubeMap : public Texture
{
private:
	static std::vector<std::string> fileSuffixes;
protected:

public:
	CubeMap() = default;
	
	CubeMap(const Texture& other)
	{
		if(other.getType() != TextureType::CUBE_MAP)
		{
			throw std::exception("CubeMap : Can't convert not CubeMap's type texture to CubeMap.");
		}

		this->id = other.getId();
		this->directory = other.getDirectory();
		this->fileName = other.getFileName();
		this->type = other.getType();
	}
	
	void loadTextureAndSetFields(const std::string& fileName, const std::string& directory) override;
};

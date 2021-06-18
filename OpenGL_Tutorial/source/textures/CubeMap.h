#pragma once

#include <vector>

#include "Texture.h"

class CubeMap : public Texture
{
private:
	static std::vector<std::string> fileSuffixes;
public:
	void loadTextureAndSetFields(const std::string& fileName, const std::string& directory) override;
};

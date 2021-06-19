#pragma once

#include "WorldObject.h"
#include "../../shader/ShaderLoader.h"

class DrawableObject : public WorldObject
{
protected:
	Shader* shader;
public:
	DrawableObject(const std::string& shaderName, const bool& shaderWithGeometry = false)
		:shader( ShaderLoader::getInstance()->getOrLoad(shaderName, FilePaths::getPathToShaderFolderWithTrailingSplitter(), shaderWithGeometry) )
	{
	}

	virtual void draw(Renderer* renderer) = 0;
};

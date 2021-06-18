#pragma once

#include "WorldObject.h"
#include "../model/ModelTransformations.h"
#include "../renderer/Renderer.h"
#include "../shader/Shader.h"
#include "../shader/ShaderLoader.h"
#include "../utility/FilePaths.h"

class DrawableObject : public WorldObject
{
protected:
	Shader* shader;
	ModelTransformations transformations;
public:
	DrawableObject(ModelTransformations transformations, const std::string& shaderName)
		:transformations(transformations),
		 shader( ShaderLoader::getInstance()->getOrLoadByName(shaderName, FilePaths::getPathToShaderFolderWithTrailingSplitter()) )
	{
	}
	
	virtual ~DrawableObject() = default;

	virtual void draw(Renderer* renderer) = 0;
};

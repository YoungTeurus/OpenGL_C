#pragma once

#include "WorldObject.h"
#include "../../shader/ShaderLoader.h"

class DrawableObject : public WorldObject
{
private:
	bool visible = true;
protected:
	Shader* shader;
	virtual void drawAction(Renderer* renderer) = 0;
public:
	DrawableObject(const std::string& shaderName, const bool& shaderWithGeometry = false)
		:shader( ShaderLoader::getInstance()->getOrLoad(shaderName, FilePaths::getPathToShaderFolderWithTrailingSplitter(), shaderWithGeometry) )
	{
	}

	void draw(Renderer* renderer)
	{
		if(visible)
		{
			drawAction(renderer);
		}
	}


	bool isVisible() const
	{
		return visible;
	}

	void setVisible(bool visible)
	{
		this->visible = visible;
	}
};

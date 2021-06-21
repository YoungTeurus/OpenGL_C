#pragma once

#include "WorldObject.h"
#include "../../shader/ShaderLoader.h"

class DrawableObject : public WorldObject
{
private:
	bool visible = true;
protected:
	float shininess = 64.0f;
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

	void setShader(Shader* shader)
	{
		this->shader = shader;
	}

	bool isVisible() const
	{
		return visible;
	}

	void setVisible(bool visible)
	{
		this->visible = visible;
	}

	float getShininess() const
	{
		return shininess;
	}

	void setShininess(float shininess)
	{
		this->shininess = shininess;
	}
};

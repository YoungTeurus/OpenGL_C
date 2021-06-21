#pragma once

#include "WorldObject.h"
#include "../../renderer/Renderer.h"
#include "../../shader/ShaderLoader.h"

class DrawableObject : public WorldObject
{
private:
	bool visible = true;
	Renderer* attachedRenderer;
protected:
	float shininess = 64.0f;
	Shader* shader;
	virtual void drawAction() = 0;
public:
	DrawableObject(Renderer* renderer, const std::string& shaderName, const bool& shaderWithGeometry = false)
		:attachedRenderer(renderer), shader( ShaderLoader::getInstance()->getOrLoad(shaderName, FilePaths::getPathToShadersFolderWithTrailingSplitter(), shaderWithGeometry) )
	{
	}

	void draw()
	{
		if (attachedRenderer == nullptr)
		{
			throw exception("DrawableObject::draw: renderer wasn't attached before drawing!");
		}
		if(visible)
		{
			drawAction();
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

	Renderer* getAttachedRenderer() const
	{
		return attachedRenderer;
	}

	void setAttachedRenderer(Renderer* renderer)
	{
		attachedRenderer = renderer;
	}
};

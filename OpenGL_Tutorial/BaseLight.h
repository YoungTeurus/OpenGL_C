#pragma once
#include <string>
#include "classes/Shader.h"

enum class LightType
{
	None = 0,
	Directional = 1,
	Point = 2,
	Spot = 3,
	Ambient = 4
};

class BaseLight
{
private:
	std::string name;
	bool isActive = true;

	LightType type = LightType::None;

public:
	BaseLight(const std::string& name, const LightType lightType): name(name), type(lightType){}
	virtual ~BaseLight() = default;

	void turnOn()
	{
		setIsActive(true);
	}
	void turnOff()
	{
		setIsActive(false);
	}

	bool getIsActive() const
	{
		return isActive;
	}
	void setIsActive(bool isActive)
	{
		this->isActive = isActive;
	}

	LightType getType() const
	{
		return type;
	}
	
	int useAndReturnSuccess(Shader* shader, int lightNumber)
	{
		if(!getIsActive())
		{
			return 0;
		}

		std::string lightNumberString = std::to_string(lightNumber);
		setShaderParameters(shader, lightNumberString);
		
		return 1;
	}

	virtual void setShaderParameters(Shader* shader, std::string lightNumber)
	{
		shader->setInt("lights[" + lightNumber + "].type", int(type));
	}
};


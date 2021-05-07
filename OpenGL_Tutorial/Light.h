#pragma once
#include "ShaderedWorldObject.h"


class Light : public WorldObject
{
protected:
	int id_;

	glm::vec3 ambient_;
	glm::vec3 diffuse_;
	glm::vec3 specular_;

public:
	static int max_id;
	
	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
		const float pos_x = 0, const float pos_y = 0, const float pos_z = 0)
			: WorldObject(pos_x, pos_y, pos_z), ambient_(ambient), diffuse_(diffuse), specular_(specular)
	{
		id_ = max_id++;
	}
	
	void draw() const;

	virtual void use(const Shader* shader) const = 0;
	virtual ~Light() = default;
};

int Light::max_id = 0;

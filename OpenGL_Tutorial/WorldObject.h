#pragma once

#include "classes/Shader.h"
#include "classes/Texture.h"

struct Material
{
	Texture *diffuseMap;
	Texture *specularMap;
	Texture *emissionMap;
	float shineiness;
	
	void use(const Shader *shader) const
	{
		// ��������� (������) ����� �������� � �������� 0
		glActiveTexture(GL_TEXTURE0);
		diffuseMap->use();
		glActiveTexture(GL_TEXTURE1);
		specularMap->use();
		glActiveTexture(GL_TEXTURE2);
		emissionMap->use();
		
		shader->setInt("material.diffuse", 0);  // ���������, ��� ��� ����� �������� ������������ �������� 0
		shader->setInt("material.specular", 1);  // ���������, ��� ��� ����� �������� ������������ �������� 1
		shader->setInt("material.emission", 2);  // ���������, ��� ��� ����� �������� ������������ �������� 2
		shader->setFloat("material.shininess", shineiness);
	}
};

class WorldObject
{
protected:
	glm::vec3 position_;
public:
	// ��������� �������
	void draw() const;

	WorldObject(const float pox_x = 0, const float pos_y = 0, const float pos_z = 0) : position_(pox_x, pos_y, pos_z)
	{
	}
	
	virtual ~WorldObject()
	{
		// Mesh � texture ����� �������������� ������?
		// delete texture_;
		// delete _mesh;
	};
};


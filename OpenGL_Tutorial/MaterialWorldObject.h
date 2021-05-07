#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Light.h"
#include "Mesh.h"
#include "ShaderedWorldObject.h"

class MaterialWorldObject : public ShaderedWorldObject
{
protected:
	Mesh *_mesh = nullptr;
	const Material *_material = nullptr;

	glm::vec3 scale_;
public:
	MaterialWorldObject(const Shader* shader, const Material* material,
		const float pos_x = 0, const float pos_y = 0, const float pos_z = 0,
		const float scale_x = 1, const float scale_y = 1, const float scale_z = 1)
			: ShaderedWorldObject(shader, pos_x, pos_y, pos_z), _material(material), scale_(scale_x, scale_y, scale_z)
	{
	}
	
	void draw(glm::mat4 view, glm::mat4 projection, glm::vec3 viewPos, const std::list<Light*> &lights) const
	{
		_shader->use();
		_material->use(_shader);

		for (const auto& light : lights)
		{
			light->use(_shader);
		}

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position_);
		model = glm::scale(model, scale_);
		
		glm::mat4 transformation = projection * view * model;
		
		// Матрица нормалей:
		glm::mat3 normal = glm::transpose(glm::inverse(model));
		
		_shader->setFloatMat4("model", glm::value_ptr(model));
		_shader->setFloatMat4("view", glm::value_ptr(view));
		_shader->setFloatMat4("projection", glm::value_ptr(projection));
		_shader->setFloatMat4("transformation", glm::value_ptr(transformation));
		_shader->setFloatMat3("normal", glm::value_ptr(normal));
		_shader->setFloatVec3("viewPos", viewPos);

		_mesh->use();
	};

	virtual ~MaterialWorldObject() = default;
};

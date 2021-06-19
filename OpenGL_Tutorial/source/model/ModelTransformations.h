#pragma once
#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>

struct ModelTransformations
{
	glm::vec3 position;
	glm::vec3 rotationVector;
	float rotationAngleDegrees;
	glm::vec3 scale;

	ModelTransformations()
		:ModelTransformations(glm::vec3(0.0f))
	{
		
	}

	ModelTransformations(const glm::vec3& position)
		:ModelTransformations(position, glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, glm::vec3(1.0f))
	{
	}

	ModelTransformations(const glm::vec3& position, const glm::vec3& rotationVector, float rotationAngleDegrees,
		const glm::vec3& scale)
		: position(position),
		  rotationVector(rotationVector),
		  rotationAngleDegrees(rotationAngleDegrees),
		  scale(scale)
	{
	}

	glm::mat4 createModelMatrixWithTransformations() const
	{
		glm::mat4 model = glm::mat4(1.0f);
		return getModelWithAppliedTransformations(model);
	}

	glm::mat4 getModelWithAppliedTransformations(glm::mat4 model) const{
		glm::mat4 transformedModel = model;

		transformedModel = glm::translate(transformedModel, position);
		transformedModel = glm::rotate(transformedModel, glm::radians(rotationAngleDegrees), rotationVector);
		transformedModel = glm::scale(transformedModel, scale);

		return transformedModel;
	}
};

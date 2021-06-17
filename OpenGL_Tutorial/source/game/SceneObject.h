#pragma once
#include <string>
#include <glm/ext/matrix_transform.hpp>

#include "../model/Model.h"
#include "../shader/Shader.h"

using namespace std;

struct SceneObjectTransformations
{
	glm::vec3 position;
	glm::vec3 rotationVector;
	float rotationAngleDegrees;
	glm::vec3 scale;


	SceneObjectTransformations(const glm::vec3& position, const glm::vec3& rotationVector, float rotationAngleDegrees,
		const glm::vec3& scale)
		: position(position),
		  rotationVector(rotationVector),
		  rotationAngleDegrees(rotationAngleDegrees),
		  scale(scale)
	{
	}

	SceneObjectTransformations(const glm::vec3& position)
		:position(position), rotationVector(0.0f, 1.0f, 0.0f),
		 rotationAngleDegrees(0.0f), scale(1.0f)
	{
	}

	SceneObjectTransformations()
		:position(0.0f), rotationVector(0.0f, 1.0f, 0.0f),
		 rotationAngleDegrees(0.0f), scale(1.0f)
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

class SceneObject
{
public:
	string name;


	SceneObject(const string& name)
		: name(name)
	{
	}

	virtual ~SceneObject() = default;

	virtual void draw() const = 0;
};
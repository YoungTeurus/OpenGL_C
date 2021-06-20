#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "../../model/VAOBuilder.h"
#include "../../renderer/Renderer.h"
#include "../interfaces/DrawableObject.h"
#include "../interfaces/DrawableUpdatableObject.h"
#include "../interfaces/PositionedObject.h"
#include "../interfaces/UpdatableObject.h"

struct Particle
{
	glm::vec3 position, velocity;
	glm::vec4 color;
	float life;

	Particle()
		:position(0.0f), velocity(0.0f), color(1.0f), life(0.0f)
	{
	}

	void spawn(const glm::vec3& position, const glm::vec3& velocity, const glm::vec4& color, const float& lifeDuration)
	{
		this->position = position;
		this->velocity = velocity;
		this->color = color;
		this->life = lifeDuration;
	}

	void update(const float& deltaTime)
	{
		if (isAlive()) {
			life -= deltaTime;
			position += velocity * deltaTime;
			color.a -= deltaTime * 0.025f;
		}
	}

	ModelTransformations getTransformations()
	{
		return ModelTransformations{position};
	}

	bool isAlive()
	{
		return life > 0.0f;
	}
};

class ParticleGenerator : public PositionedObject, public DrawableUpdatableObject
{
private:
	VOsAndIndices* particleVOsAndIndices = VAOBuilder::getInstance()->getCube();
	std::vector<Particle> particles;
	
	unsigned updateNumOfNewParticles;
	unsigned numOfParticles;
	float particleScale;
	float particleLifeDuration;
	float possibleLifeDeviations;
	glm::vec3 particleColor;
	glm::vec3 possibleColorDeviations;
	glm::vec3 possiblePositionDeviations;
	glm::vec3 particleVelocity;
	glm::vec3 possibleVelocityDeviations;

	float lastUpdateTime = 0.0f;
	unsigned indexOfLastUsedParticle = 0;
	unsigned getIndexOfFirstUnusedParticle()
	{
		for (unsigned i = indexOfLastUsedParticle; i < numOfParticles; i++)
		{
			if (! particles[i].isAlive())
			{
				indexOfLastUsedParticle = i;
				return i;
			}
		}

		for (unsigned i = 0; i < indexOfLastUsedParticle; i++)
		{
			if (! particles[i].isAlive())
			{
				indexOfLastUsedParticle = i;
				return i;
			}
		}

		indexOfLastUsedParticle = 0;
		return 0;
	}

	void respawnParticle(unsigned particleId)
	{
		Particle* particle = &particles.at(particleId);
		if (particle->isAlive())
		{
			return;
		}
		
		float positionXDeviation =  (rand() % ((int)(possiblePositionDeviations.x * 100) + 1) - ((int)(possiblePositionDeviations.x * 50) + 1)) / 100.0f;
		float positionYDeviation =  (rand() % ((int)(possiblePositionDeviations.y * 100) + 1) - ((int)(possiblePositionDeviations.y * 50) + 1)) / 100.0f;
		float positionZDeviation =  (rand() % ((int)(possiblePositionDeviations.z * 100) + 1) - ((int)(possiblePositionDeviations.z * 50) + 1)) / 100.0f;
		const glm::vec3 position = getPosition() + glm::vec3(positionXDeviation, positionYDeviation, positionZDeviation);

		float velocityXDeviation =  (rand() % ((int)(possibleVelocityDeviations.x * 100) + 1) - ((int)(possibleVelocityDeviations.x * 50) + 1)) / 100.0f;
		float velocityYDeviation =  (rand() % ((int)(possibleVelocityDeviations.y * 100) + 1) - ((int)(possibleVelocityDeviations.y * 50) + 1)) / 100.0f;
		float velocityZDeviation =  (rand() % ((int)(possibleVelocityDeviations.z * 100) + 1) - ((int)(possibleVelocityDeviations.z * 50) + 1)) / 100.0f;
		const glm::vec3 velocity = particleVelocity + glm::vec3(velocityXDeviation, velocityYDeviation, velocityZDeviation);
		
		float colorRDeviation =  (rand() % ((int)(possibleColorDeviations.r * 100) + 1) - ((int)(possibleColorDeviations.r * 50) + 1)) / 100.0f;
		float colorGDeviation =  (rand() % ((int)(possibleColorDeviations.g * 100) + 1) - ((int)(possibleColorDeviations.g * 50) + 1)) / 100.0f;
		float colorBDeviation =  (rand() % ((int)(possibleColorDeviations.b * 100) + 1) - ((int)(possibleColorDeviations.b * 50) + 1)) / 100.0f;
		const glm::vec4 color = glm::vec4(particleColor + glm::vec3(colorRDeviation, colorGDeviation, colorBDeviation), 1.0f);
		
		float lifeDuration = particleLifeDuration + (rand() % ((int)(possibleLifeDeviations * 100) + 1) - ((int)(possibleLifeDeviations * 50) + 1)) / 100.0f;
		
		particle->spawn(position, velocity, color, lifeDuration);
	}

public:
	ParticleGenerator(const ModelTransformations& transformations,
		const unsigned& numOfParticles, const unsigned& numOfNewParticles,
		const float& particleScale,
		const float& particleLifeDuration, const float& possibleLifeDeviations,
		const glm::vec3& particleColor, const glm::vec3& possibleColorDeviations,
		const glm::vec3& possiblePositionDeviations,
		const glm::vec3& particleVelocity, const glm::vec3& possibleVelocityDeviations
	)
		:PositionedObject(transformations), DrawableUpdatableObject("particles"),
		 numOfParticles(numOfParticles), updateNumOfNewParticles(numOfNewParticles),
		 particleScale(particleScale),
		 particleLifeDuration(particleLifeDuration), possibleLifeDeviations(possibleLifeDeviations),
		 particleColor(particleColor), possibleColorDeviations(possibleColorDeviations),
		 possiblePositionDeviations(possiblePositionDeviations),
		 particleVelocity(particleVelocity), possibleVelocityDeviations(possibleVelocityDeviations)
	{
		for(unsigned i = 0; i < numOfParticles; i++)
		{
			particles.push_back(Particle());
		}
	}

	void drawAction(Renderer* renderer) override
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		shader->use();
		shader->setFloatMat4("pv", renderer->getPV());
		shader->setFloat("uScale", particleScale);
		for (auto && particle : particles)
		{
			if(particle.isAlive())
			{
				shader->setFloatMat4("model", particle.getTransformations().createModelMatrixWithTransformations());
				shader->setFloatVec4("uColor", particle.color);

				glBindVertexArray(particleVOsAndIndices->vao);
				glDrawElements(GL_TRIANGLES, particleVOsAndIndices->indices.size(), GL_UNSIGNED_INT, 0);
			}
		}

		glDisable(GL_BLEND);
	}

	void update(const float& currentTime) override
	{
		UpdatableObject::update(currentTime);

		// TODO: передавать в UpdatableObject-ы render, чтобы они сами выбирали формат времени?
		const float deltaTime = currentTime - lastUpdateTime;
		lastUpdateTime = currentTime;

		for(unsigned i = 0; i < updateNumOfNewParticles; i++)
		{
			unsigned indexOfUnusedParticle = getIndexOfFirstUnusedParticle();
			respawnParticle(indexOfUnusedParticle);
		}
		for (auto &particle : particles)
		{
			particle.update(deltaTime);
		}
	}
};

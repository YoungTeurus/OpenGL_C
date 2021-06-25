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
	float scale;

	Particle()
		:position(0.0f), velocity(0.0f), color(1.0f), life(0.0f), scale(1.0f)
	{
	}

	void spawn(const glm::vec3& position, const glm::vec3& velocity, const glm::vec4& color, const float& lifeDuration, const float& scale)
	{
		this->position = position;
		this->velocity = velocity;
		this->color = color;
		this->life = lifeDuration;
		this->scale = scale;
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

struct ParticleGeneratorSettings
{
	glm::vec3 initialPosition;
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
};

class ParticleGenerator : public PositionedObject, public DrawableUpdatableObject, public AnimatedObject<ParticleGenerator>
{
private:
	VOsAndIndices* particleVOsAndIndices = VAOBuilder::getInstance()->getCube();
	std::vector<Particle> particles;

	bool isActive = false;
	
	unsigned	updateNumOfNewParticles;
	unsigned	numOfParticles;
	float		particleScale;
	float		particleLifeDuration;
	float		possibleLifeDeviations;
	glm::vec3	particleColor;
	glm::vec3	possibleColorDeviations;
	glm::vec3	possiblePositionDeviations;
	glm::vec3	particleVelocity;
	glm::vec3	possibleVelocityDeviations;

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
		
		particle->spawn(position, velocity, color, lifeDuration, particleScale);
	}

	void createNewParticles(unsigned count)
	{
		for(unsigned i = 0; i < count; i++)
		{
			particles.push_back(Particle());
		}
	}
public:
	ParticleGenerator(Renderer* renderer, const ModelTransformations& transformations,
		const unsigned& numOfParticles, const unsigned& numOfNewParticles,
		const float& particleScale,
		const float& particleLifeDuration, const float& possibleLifeDeviations,
		const glm::vec3& particleColor, const glm::vec3& possibleColorDeviations,
		const glm::vec3& possiblePositionDeviations,
		const glm::vec3& particleVelocity, const glm::vec3& possibleVelocityDeviations
	)
		:PositionedObject(transformations), DrawableUpdatableObject(renderer, "particles"),
		 numOfParticles(numOfParticles), updateNumOfNewParticles(numOfNewParticles),
		 particleScale(particleScale),
		 particleLifeDuration(particleLifeDuration), possibleLifeDeviations(possibleLifeDeviations),
		 particleColor(particleColor), possibleColorDeviations(possibleColorDeviations),
		 possiblePositionDeviations(possiblePositionDeviations),
		 particleVelocity(particleVelocity), possibleVelocityDeviations(possibleVelocityDeviations)
	{
		createNewParticles(numOfParticles);
	}

	void updateSettings(const ParticleGeneratorSettings& settings)
	{
		setPosition(settings.initialPosition);
		this->updateNumOfNewParticles = settings.updateNumOfNewParticles;
		setNumOfParticles(settings.numOfParticles);
		this->particleScale = settings.particleScale;
		this->particleLifeDuration = settings.particleLifeDuration;
		this->possibleLifeDeviations = settings.possibleLifeDeviations;
		this->particleColor = settings.particleColor;
		this->possibleColorDeviations = settings.possibleColorDeviations;
		this->possiblePositionDeviations = settings.possiblePositionDeviations;
		this->particleVelocity = settings.particleVelocity;
		this->possibleVelocityDeviations = settings.possibleVelocityDeviations;
	}

	void activate()
	{
		this->isActive = true;
	}

	void deactivate()
	{
		this->isActive = false;
	}

	void drawAction() override
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		shader->use();
		shader->setFloatMat4("pv", getAttachedRenderer()->getPV());
		for (auto && particle : particles)
		{
			if(particle.isAlive())
			{
				shader->setFloat("uScale", particle.scale);
				shader->setFloatMat4("model", particle.getTransformations().createModelMatrixWithTransformations());
				shader->setFloatVec4("uColor", particle.color);

				glBindVertexArray(particleVOsAndIndices->vao);
				glDrawElements(GL_TRIANGLES, particleVOsAndIndices->indices.size(), GL_UNSIGNED_INT, 0);
			}
		}

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void update(const float& deltaTime) override
	{
		DrawableUpdatableObject::update(deltaTime);
		AnimatedObject::update(deltaTime);

		if (isActive){
			for(unsigned i = 0; i < updateNumOfNewParticles; i++)
			{
				unsigned indexOfUnusedParticle = getIndexOfFirstUnusedParticle();
				respawnParticle(indexOfUnusedParticle);
			}
		}
		for (auto &particle : particles)
		{
			particle.update(deltaTime);
		}
	}


	void setUpdateNumOfNewParticles(unsigned updateNumOfNewParticles)
	{
		this->updateNumOfNewParticles = updateNumOfNewParticles;
	}

	void setNumOfParticles(unsigned numOfParticles)
	{
		const int delta = (int)numOfParticles - (int)this->numOfParticles;
		if (delta > 0)
		{
			createNewParticles((unsigned)delta);
		}
		this->numOfParticles = numOfParticles;
	}

	void setParticleScale(float particleScale)
	{
		this->particleScale = particleScale;
	}

	void setParticleLifeDuration(float particleLifeDuration)
	{
		this->particleLifeDuration = particleLifeDuration;
	}

	void setPossibleLifeDeviations(float possibleLifeDeviations)
	{
		this->possibleLifeDeviations = possibleLifeDeviations;
	}

	void setParticleColor(glm::vec3 particleColor)
	{
		this->particleColor = particleColor;
	}

	void setPossibleColorDeviations(glm::vec3 possibleColorDeviations)
	{
		this->possibleColorDeviations = possibleColorDeviations;
	}

	void setPossiblePositionDeviations(glm::vec3 possiblePositionDeviations)
	{
		this->possiblePositionDeviations = possiblePositionDeviations;
	}

	void setParticleVelocity(glm::vec3 particleVelocity)
	{
		this->particleVelocity = particleVelocity;
	}

	void setPossibleVelocityDeviations(glm::vec3 possibleVelocityDeviations)
	{
		this->possibleVelocityDeviations = possibleVelocityDeviations;
	}
};

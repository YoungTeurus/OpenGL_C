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
	// Texture* particleTexture = TexturesLoader::getInstance()->getOrLoad2DTexture("particle.png");
	
	unsigned updateNumOfNewParticles = 25;
	unsigned numOfParticles = 200;
	std::vector<Particle> particles;

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
		
		float randomX = ((rand() % 10) - 5) / 10.0f;
		float randomY = ((rand() % 10) - 5) / 10.0f;
		float randomZ = ((rand() % 10) - 5) / 10.0f;
		// float randColor = 0.5f + (rand() % 100) / 100.0f;
		float randLife = 2.5f + (rand() % 251) / 100.0f;

		const glm::vec3 randomPosition = getPosition() + glm::vec3(randomX, randomY, randomZ);
		const glm::vec3 velocity = glm::vec3(0.0f + randomX, -1.0f + randomY, 0.0f + randomZ);
		// const glm::vec4 randomColor = glm::vec4(randColor, randColor, randColor, 1.0f);
		const glm::vec4 randomColor = glm::vec4(5.0f, 2.5f, 0.0f, 1.0f);
		
		particle->spawn(randomPosition, velocity, randomColor, randLife);
	}

public:
	ParticleGenerator(const ModelTransformations& transformations)
		:PositionedObject(transformations), DrawableUpdatableObject("particles")
	{
		for(unsigned i = 0; i < numOfParticles; i++)
		{
			particles.push_back(Particle());
		}
	}

	void draw(Renderer* renderer) override
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		shader->use();
		shader->setFloatMat4("pv", renderer->getPV());
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

		// TODO: ���������� � UpdatableObject-� render, ����� ��� ���� �������� ������ �������?
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

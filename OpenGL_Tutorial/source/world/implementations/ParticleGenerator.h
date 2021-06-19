#pragma once
#include <vector>
#include <glm/glm.hpp>



#include "../../model/VAOBuilder.h"
#include "../../renderer/Renderer.h"
#include "../interfaces/DrawableObject.h"
#include "../interfaces/PositionedObject.h"
#include "../interfaces/UpdatableObject.h"

struct Particle
{
	glm::vec2 position, velocity;
	glm::vec4 color;
	float life;

	Particle()
		:position(0.0f), velocity(0.0f), color(1.0f), life(0.0f)
	{
	}

	void spawn(const glm::vec2& position, const glm::vec2& velocity, const glm::vec4& color, const float& lifeDuration)
	{
		this->position = position;
		this->velocity = velocity;
		this->color = color;
		this->life = lifeDuration;
	}

	void update(const float& deltaTime)
	{
		life -= deltaTime;
		if (isAlive()) {
			position += velocity * deltaTime;
			color.a -= deltaTime * 2.5f;
		}
	}

	bool isAlive()
	{
		return life > 0.0f;
	}
};

class ParticleGenerator : public UpdatableObject, public PositionedObject, public DrawableObject
{
private:
	VOsAndIndices* particleVOsAndIndices = VAOBuilder::getInstance()->get2DQuad();
	Texture* particleTexture = TexturesLoader::getInstance()->getOrLoad2DTexture("particle.png");
	
	unsigned updateNumOfNewParticles = 2;
	unsigned numOfParticles = 500;
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

	void respawnParticle(Particle particle)
	{
		float random = ((rand() % 100) - 50) / 10.0f;
		float randColor = 0.5f + ((rand() % 100) / 100.0f);
		particle.spawn(getPosition() + random, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(randColor, randColor, randColor, 1.0f), 1.0f);
	}

public:
	ParticleGenerator(const ModelTransformations& transformations)
		:PositionedObject(transformations), DrawableObject("particles")
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
		shader->setFloatMat4("projection", renderer->getOrthoProjection());
		glActiveTexture(GL_TEXTURE0);
		shader->setInt("sprite", 0);
		glBindTexture(GL_TEXTURE_2D, particleTexture->getId());
		for (auto && particle : particles)
		{
			if(particle.isAlive())
			{
				shader->setFloatVec2("uOffset", particle.position);
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
			unsigned unusedParticle = getIndexOfFirstUnusedParticle();
			respawnParticle(particles[unusedParticle]);
		}
		for (auto particle : particles)
		{
			particle.update(deltaTime);
		}
	}
};

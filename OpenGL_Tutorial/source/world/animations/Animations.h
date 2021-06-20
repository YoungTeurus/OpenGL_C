#pragma once
#include "PackedAnimation.h"
#include "../implementations/Tank.h"
#include "DelayedAction.h"
#include "InterpolateValueAnimation.h"
#include "../implementations/ParticleGenerator.h"

class Animations
{
public:
	template<class T>
	static Animation<T>* rotateToAngle(T *object, const float& duration, const float& endValue)
	{
		static_cast<PositionedObject*>((T*)0);
		float startAngle = object->getRotationAngleDegrees();
		float endAngle = endValue;

		if (abs(endValue - startAngle) > 180.0f)
		{
			if (startAngle <= endValue)
			{
				startAngle = startAngle + 360.0f;
			} else
			{
				endAngle = endAngle + 360.0f;
			}
		}

		void (PositionedObject::* setFunction)(float) = &PositionedObject::setRotationAngleDegrees;

		auto *animation = new FloatAnimation<T>(
			object,
			duration,
			startAngle,
			endAngle,
			setFunction
		);
		
		return animation;
	}

	static Animation<Tank>* blowTank(Tank* tank, const float& duration, const float& explosionForce)
	{
		auto *animation = new PackedAnimation<Tank>(tank, duration);

		void (Tank::* setExplosionMagnitude)(float) = &Tank::setExplosionMagnitude;
		void (Tank::* setTimeSinceExplosion)(float) = &Tank::setTimeSinceExplosion;
		void (Tank::* setVisible)(bool) = &Tank::setVisible;

		animation->addAnimation(
			new FloatAnimation<Tank>(
				tank,
				duration,
				0.0f,
				explosionForce,
				setExplosionMagnitude
			)
		);

		animation->addAnimation(
			new FloatAnimation<Tank>(
				tank,
				duration,
				0.0f,
				duration,
				setTimeSinceExplosion
			)
		);

		animation->addAnimation(
			new DelayedSingleParameterAction<Tank, bool>(
				tank,
				duration,
				setVisible,
				false
			)
		);

		return animation;
	}

	static Animation<ParticleGenerator>* deactivateIn(ParticleGenerator* particleGenerator, const float& delay)
	{
		void (ParticleGenerator::* deactivate)(void) = &ParticleGenerator::deactivate;

		return new DelayedVoidAction<ParticleGenerator>(
			particleGenerator,
			delay,
			deactivate
		);
	}

	static Animation<ParticleGenerator>* changeParticleSettings(ParticleGenerator* particleGenerator, const float& duration, ParticleGeneratorSettings initial, ParticleGeneratorSettings end)
	{
		auto *allAnimations = new PackedAnimation<ParticleGenerator>(particleGenerator, duration);

		allAnimations->addAnimation(
			new Vec3Animation<ParticleGenerator>(
				particleGenerator, duration,
				initial.initialPosition,
				end.initialPosition,
				&ParticleGenerator::setPosition
			)
		);
		
		allAnimations->addAnimation(
			new UnsignedAnimation<ParticleGenerator>(
				particleGenerator, duration,
				initial.updateNumOfNewParticles,
				end.updateNumOfNewParticles,
				&ParticleGenerator::setUpdateNumOfNewParticles
			)
		);
		
		allAnimations->addAnimation(
			new UnsignedAnimation<ParticleGenerator>(
				particleGenerator, duration,
				initial.numOfParticles,
				end.numOfParticles,
				&ParticleGenerator::setNumOfParticles
			)
		);

		allAnimations->addAnimation(
			new FloatAnimation<ParticleGenerator>(
				particleGenerator, duration,
				initial.particleScale,
				end.particleScale,
				&ParticleGenerator::setParticleScale
			)
		);

		allAnimations->addAnimation(
			new FloatAnimation<ParticleGenerator>(
				particleGenerator, duration,
				initial.particleLifeDuration,
				end.particleLifeDuration,
				&ParticleGenerator::setParticleLifeDuration
			)
		);

		allAnimations->addAnimation(
			new FloatAnimation<ParticleGenerator>(
				particleGenerator, duration,
				initial.possibleLifeDeviations,
				end.possibleLifeDeviations,
				&ParticleGenerator::setPossibleLifeDeviations
			)
		);

		allAnimations->addAnimation(
			new Vec3Animation<ParticleGenerator>(
				particleGenerator, duration,
				initial.particleColor,
				end.particleColor,
				&ParticleGenerator::setParticleColor
			)
		);

		allAnimations->addAnimation(
			new Vec3Animation<ParticleGenerator>(
				particleGenerator, duration,
				initial.possibleColorDeviations,
				end.possibleColorDeviations,
				&ParticleGenerator::setPossibleColorDeviations
			)
		);

		allAnimations->addAnimation(
			new Vec3Animation<ParticleGenerator>(
				particleGenerator, duration,
				initial.possiblePositionDeviations,
				end.possiblePositionDeviations,
				&ParticleGenerator::setPossiblePositionDeviations
			)
		);

		allAnimations->addAnimation(
			new Vec3Animation<ParticleGenerator>(
				particleGenerator, duration,
				initial.particleVelocity,
				end.particleVelocity,
				&ParticleGenerator::setParticleVelocity
			)
		);

		allAnimations->addAnimation(
			new Vec3Animation<ParticleGenerator>(
				particleGenerator, duration,
				initial.possibleVelocityDeviations,
				end.possibleVelocityDeviations,
				&ParticleGenerator::setPossibleVelocityDeviations
			)
		);

		return allAnimations;
	}

	static Animation<Tank>* rotateTurretToAngle(Tank* tank, const float& duration, const float& endAngle)
	{
		float startAngle = tank->getTurretRotationAngleDegrees();
		float _endAngle = Tank::clampAngleTo360(endAngle);

		if (abs(_endAngle - startAngle) > 180.0f)
		{
			if (startAngle <= _endAngle)
			{
				startAngle = startAngle + 360.0f;
			} else
			{
				_endAngle = endAngle + 360.0f;
			}
		}
		
		void (Tank::* setTurretRotation)(float) = &Tank::setTurretRotationAngleDegrees;

		auto *animation = new FloatAnimation<Tank>(
			tank,
			duration,
			startAngle,
			_endAngle,
			setTurretRotation
		);
		
		return animation;
	}
};

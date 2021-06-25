#pragma once

template<class T>
class Animation
{
private:
	void checkAndSetIfEnded(const float& deltaTime)
	{
		if (timeSinceStart > length)
		{
			hasEnded = true;
		}
	}

	void updateTimeSinceStart(const float& deltaTime)
	{
		timeSinceStart += deltaTime;
	}

protected:
	bool hasStarted = false;
	bool hasEnded = false;

	T *object;
	float length;  // Длительность анимации
	float timeSinceStart = 0.0f;

	Animation(T *object, const float& length)
		:object(object), length(length)
	{
	}

	virtual void actCore(const float& deltaTime) = 0;
public:
	void act(const float& deltaTime)
	{
		if (!this->hasStarted)
		{
			this->hasStarted = true;
		}
		if (this->hasEnded)
		{
			return;
		}

		updateTimeSinceStart(deltaTime);

		actCore(deltaTime);

		checkAndSetIfEnded(deltaTime);
	}
	
	virtual void endImmediately() = 0;

	virtual ~Animation() = default;

	bool isHasEnded() const
	{
		return hasEnded;
	}
};

#pragma once

template<class T>
class Animation
{
private:
	void checkAndSetIfEnded(const float& currentTime)
	{
		if (remainedLength <= 0.0f)
		{
			hasEnded = true;
		}
	}

protected:
	bool hasStarted = false;
	bool hasEnded = false;

	T *object;
	float length;  // Длительность анимации
	float remainedLength;

	Animation(T *object, const float& length)
		:object(object), length(length), remainedLength(length)
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

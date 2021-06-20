#pragma once

template<class T>
class Animation
{
private:
	void checkAndSetIfEnded(const float& currentTime)
	{
		if (currentTime - startTime > length)
		{
			hasEnded = true;
		}
	}

protected:
	bool hasStarted = false;
	bool hasEnded = false;

	T *object;
	float length;  // Длительность анимации
	float startTime;  // Время начала анимации

	Animation(T *object, const float& length)
		:object(object), length(length), startTime(0.0f)
	{
	}

	virtual void actCore(const float& currentTime) = 0;
public:
	void act(const float& currentTime)
	{
		if (!this->hasStarted)
		{
			this->startTime = currentTime;
			this->hasStarted = true;
		}
		if (this->hasEnded)
		{
			return;
		}

		actCore(currentTime);

		checkAndSetIfEnded(currentTime);
	}
	
	virtual void endImmediately() = 0;

	virtual ~Animation() = default;

	bool isHasEnded() const
	{
		return hasEnded;
	}
};

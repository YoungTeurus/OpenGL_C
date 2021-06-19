#pragma once


class Animation
{
protected:
	bool hasStarted = false;
	bool hasEnded = false;

	PositionedObject *object;
	float length;  // ������������ ��������
	float startTime;  // ����� ������ ��������

	Animation(PositionedObject *object, const float& length)
		:object(object), length(length), startTime(0.0f)
	{
	}

	void checkAndSetIfEnded(const float& currentTime)
	{
		if (currentTime - startTime > length)
		{
			hasEnded = true;
		}
	}

	virtual float getInterpolationValue(const float& currentTime) const = 0;
public:
	virtual void act(const float& currentTime) = 0;
	virtual void endImmediately() = 0;


	bool isHasEnded() const
	{
		return hasEnded;
	}
};
#pragma once

class WorldObject
{
private:
	bool canBeRemoved = false;
public:
	virtual ~WorldObject() = default;

	static bool isReadyToBeRemoved(WorldObject* obj)
	{
		return obj->canBeRemoved;
	}

	void signalCanBeRemoved()
	{
		canBeRemoved = true;
	}
};

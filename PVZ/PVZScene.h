#pragma once

#include "GameManager.h"
#include "AABB.h"

#include <list>

class Plant;
class Zombie;

namespace sf 
{
	class Event;
}

class PVZScene : public Scene
{
public:
	enum Tag
	{
		PLANT,
		ZOMBIE,
		PROJECTILE
	};

	int mLaneZombieCount[3] = {0, 0, 0};		
	AABB mAreas[3];

private:
	int GetClickedArea(int x, int y) const;

public:
	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;

	bool IsZombieInArea(int index) const;
	void OnDestroyZombie(int lane);
};


#pragma once

#include "GameManager.h"

#include <list>

class Player;
class Zombie;

namespace sf 
{
	class Event;
}

struct AABB 
{
	int xMin;
	int yMin;
	int xMax;
	int yMax;
};

class PVZScene : public Scene
{
public:
	enum Tag
	{
		PLAYER_BLUE,
		PLAYER_RED,
		BALL,

		COUNT
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


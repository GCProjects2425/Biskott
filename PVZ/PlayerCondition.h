#pragma once

#include "Condition.h"
#include "Player.h"

class PlantCondition_ZombieOnLane : public Condition<Player>
{
public:
	bool OnTest(Player* owner) override;
};

class PlantCondition_NoAmmo : public Condition<Player>
{
public:
	bool OnTest(Player* owner) override;
};

class PlantCondition_FullAmmo : public Condition<Player>
{
public:
	bool OnTest(Player* owner) override;
};

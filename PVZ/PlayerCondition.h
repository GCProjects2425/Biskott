#pragma once
#include "Condition.h"
#include "Player.h"

class PlayerCondition_HasBall : public Condition<Player>
{
public:
	bool OnTest(Player* owner) override;
};

class PlayerCondition_OpponentHasBall : public Condition<Player>
{
public:
	bool OnTest(Player* owner) override;
};

class PlayerCondition_IsInDanger : public Condition<Player>
{
public:
	bool OnTest(Player* owner) override;
};
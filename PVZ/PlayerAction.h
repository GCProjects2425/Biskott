#pragma once

#include "Action.h"
#include "Player.h"

class PlayerAction_Idle : public Action<Player>
{
public:
	void OnStart(Player* pPlant) override {}
	void OnUpdate(Player* pPlant) override {}
	void OnEnd(Player* pPlant) override {}
};

class PlayerAction_Attack : public Action<Player>
{
	float mShootTimer = 0.0f;

public:
	void OnStart(Player* pPlant) override;
	void OnUpdate(Player* pPlant) override;
	void OnEnd(Player* pPlant) override {};
};

class PlayerAction_Defense : public Action<Player>
{
	float mReloadTimer = 0.0f;

public:
	void OnStart(Player* pPlant) override;
	void OnUpdate(Player* pPlant) override;
	void OnEnd(Player* pPlant) override {};

	friend Player;
};
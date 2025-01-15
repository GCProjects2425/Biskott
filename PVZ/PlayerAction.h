#pragma once

#include "Action.h"
#include "Player.h"

class PlayerAction_Idle : public Action<Player>
{
public:
	void OnStart(Player* pPlayer) override {}
	void OnUpdate(Player* pPlayer) override {}
	void OnEnd(Player* pPlayer) override {}
};

class PlayerAction_Attack : public Action<Player>
{
public:
	void OnStart(Player* pPlayer) override;
	void OnUpdate(Player* pPlayer) override;
	void OnEnd(Player* pPlayer) override {};
};

class PlayerAction_Defense : public Action<Player>
{
public:
	void OnStart(Player* pPlayer) override;
	void OnUpdate(Player* pPlayer) override;
	void OnEnd(Player* pPlayer) override {};
};

class PlayerAction_Support : public Action<Player>
{
public:
	void OnStart(Player* pPlayer) override;
	void OnUpdate(Player* pPlayer) override;
	void OnEnd(Player* pPlayer) override {};
};
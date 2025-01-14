#include "PlayerAction.h"
#include "Player.h"
#include "StateMachine.h"

#include "GameManager.h"
#include "Ball.h"
#include "PVZScene.h"

void PlayerAction_Attack::OnStart(Player* pPlant)
{
	mShootTimer = pPlant->mShootCadence;
}

void PlayerAction_Attack::OnUpdate(Player* pPlant)
{
	mShootTimer += GameManager::Get()->GetDeltaTime();

	if (mShootTimer < pPlant->mShootCadence)
		return;

	mShootTimer -= pPlant->mShootCadence;

	pPlant->Shoot();
}

void PlayerAction_Defense::OnStart(Player* pPlant)
{
	mReloadTimer = 0.f;
}

void PlayerAction_Defense::OnUpdate(Player* pPlant)
{
	mReloadTimer += GameManager::Get()->GetDeltaTime();

	if (mReloadTimer < pPlant->mReloadDuration)
		return;

	pPlant->Reload();

	pPlant->mpStateMachine->SetState(Player::Idle);
}
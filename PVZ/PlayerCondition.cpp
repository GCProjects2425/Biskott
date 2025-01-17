#include "PlayerCondition.h"


bool PlayerCondition_HasBall::OnTest(Player* pPlayer)
{
	return pPlayer->HasBall();
}

bool PlayerCondition_OpponentHasBall::OnTest(Player* pPlayer)
{
	RugbyScene* pScene = pPlayer->GetScene<RugbyScene>();
	return !pPlayer->IsTag(pScene->GetTeamWithBall());
}

bool PlayerCondition_IsInDanger::OnTest(Player* pPlayer)
{
	return !pPlayer->HasTemporaryAttribute(TemporaryAttribute::Type::Invincibility) && pPlayer->OpponentIsNear();
}

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

/*bool PlantCondition_NoAmmo::OnTest(Plant* pPlant)
{
	return pPlant->mAmmo == 0;
}

bool PlantCondition_FullAmmo::OnTest(Plant* pPlant)
{
	return pPlant->mAmmo == pPlant->mMaxAmmo;
}
*/

bool PlayerCondition_IsInDanger::OnTest(Player* pPlayer)
{
	return pPlayer->OpponentIsNear();
}

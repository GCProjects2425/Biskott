#include "PlayerCondition.h"

#include "PVZScene.h"

bool PlantCondition_ZombieOnLane::OnTest(Player* pPlant)
{
	PVZScene* pScene = pPlant->GetScene<PVZScene>();
	
	int areaIndex = pPlant->mAreaIndex;

	bool condition = pScene->IsZombieInArea(areaIndex);

	return condition;
}

bool PlantCondition_NoAmmo::OnTest(Player* pPlant)
{
	return pPlant->mAmmo == 0;
}

bool PlantCondition_FullAmmo::OnTest(Player* pPlant)
{
	return pPlant->mAmmo == pPlant->mMaxAmmo;
}

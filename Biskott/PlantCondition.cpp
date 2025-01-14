#include "PlantCondition.h"

#include "BiskottScene.h"

bool PlantCondition_ZombieOnLane::OnTest(Plant* pPlant)
{
	BiskottScene* pScene = pPlant->GetScene<BiskottScene>();
	
	int areaIndex = pPlant->mAreaIndex;

	bool condition = pScene->IsZombieInArea(areaIndex);

	return condition;
}

bool PlantCondition_NoAmmo::OnTest(Plant* pPlant)
{
	return pPlant->mAmmo == 0;
}

bool PlantCondition_FullAmmo::OnTest(Plant* pPlant)
{
	return pPlant->mAmmo == pPlant->mMaxAmmo;
}

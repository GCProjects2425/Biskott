#include "Zombie.h"

#include "BiskottScene.h"

#include <iostream>

void Zombie::OnInitialize()
{
	SetTag(BiskottScene::Tag::ZOMBIE);

	SetDirection(-1, 0, 50);
}

void Zombie::OnCollision(Entity* pCollidedWith)
{
	if (pCollidedWith->IsTag(BiskottScene::Tag::PROJECTILE) || pCollidedWith->IsTag(BiskottScene::Tag::PLANT))
	{
		GetScene<BiskottScene>()->OnDestroyZombie(mLane);

		Destroy();
	}
}

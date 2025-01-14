#include "Projectile.h"

#include "BiskottScene.h"

void Projectile::OnInitialize()
{
	SetTag(BiskottScene::Tag::PROJECTILE);

	SetDirection(1.0f, 0.0f, 100.0f);
}

void Projectile::OnUpdate()
{
	sf::Vector2f position = GetPosition(0.f, 0.f);

	int width = GetScene()->GetWindowWidth();

	if (position.x > width)
		Destroy();
}

void Projectile::OnCollision(Entity* pCollidedWith)
{
	if (pCollidedWith->IsTag(BiskottScene::Tag::ZOMBIE))
	{
		Destroy();
	}
}
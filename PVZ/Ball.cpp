#include "Ball.h"

#include "PVZScene.h"

void Ball::OnInitialize()
{
	SetTag(PVZScene::Tag::BALL);

	SetDirection(1.0f, 0.0f, 100.0f);
}

void Ball::OnUpdate()
{
	sf::Vector2f position = GetPosition(0.f, 0.f);

	int width = GetScene()->GetWindowWidth();

	if (position.x > width)
		Destroy();
}

void Ball::OnCollision(Entity* pCollidedWith)
{
	if (pCollidedWith->IsTag(PVZScene::Tag::BALL))
	{
		Destroy();
	}
}
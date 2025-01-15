#include "Ball.h"

void Ball::SetOwner(Player* owner)
{
	if (mOwner)
		mOwner->SetHasBall(false);
	owner->SetHasBall(true);
	mOwner = owner;
}

void Ball::OnUpdate()
{
    if(mOwner && !IsMoving())
    {
        SetPosition(mOwner->GetPosition().x, mOwner->GetPosition().y);
    }
    else if (mOwner && IsMoving())
    {
        GoToPosition(mOwner->GetPosition().x, mOwner->GetPosition().y, 1000.f);
		if (mOwner->GetPosition().x == GetPosition().x && mOwner->GetPosition().y == GetPosition().y)
		{
			SetIsMoving(false);
		}
    }
    // Logique de déplacement ou de gestion de la balle
}
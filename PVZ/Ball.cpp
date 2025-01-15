#include "Ball.h"

void Ball::OnUpdate()
{
    if(mOwner)
    {
        SetPosition(mOwner->GetPosition().x, mOwner->GetPosition().y);
    }
    // Logique de déplacement ou de gestion de la balle
}
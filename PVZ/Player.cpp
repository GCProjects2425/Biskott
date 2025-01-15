#include "Player.h"
#include "Debug.h"
#include "Entity.h"

void Player::MoveToPosition(float x, float y)
{
    GoToPosition(x, y, speed); // Utilise la méthode de déplacement de l'entité
}

void Player::OnUpdate()
{
    // Si le joueur a la balle, dessine un indicateur visuel
    //SetDirection(1.f, 0.f, 20.f);
    if (HasBall())
    {
        const sf::Vector2f& position = GetPosition();
        Debug::DrawCircle(position.x, position.y, 10, sf::Color::Yellow); // Indicateur de balle
    }

    // Appeler la mise à jour générale de l'entité
    //Entity::Update();
}
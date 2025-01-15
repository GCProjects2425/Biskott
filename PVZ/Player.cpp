#include "Player.h"
#include "Debug.h"
#include "Entity.h"

void Player::MoveToPosition(float x, float y)
{
    GoToPosition(x, y, speed); // Utilise la m�thode de d�placement de l'entit�
}

void Player::Update()
{
    // Si le joueur a la balle, dessine un indicateur visuel
    if (HasBall())
    {
        const sf::Vector2f& position = GetPosition();
        Debug::DrawCircle(position.x, position.y, 10, sf::Color::Yellow); // Indicateur de balle
    }

    // Appeler la mise � jour g�n�rale de l'entit�
    //Entity::Update();
}
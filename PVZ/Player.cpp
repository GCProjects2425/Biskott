#include "Player.h"
#include "Debug.h"
#include "Entity.h"
#include "RugbyScene.h"

void Player::MoveToPosition(float x, float y)
{
    GoToPosition(x, y, speed); // Utilise la méthode de déplacement de l'entité
}

void Player::OnUpdate()
{
    if (HasBall())
    {
        RugbyScene* pScene = GetScene<RugbyScene>();

		std::vector<Player*> teamPlayers;
        pScene->GetTeamPlayers(teamPlayers, mTag);

		for (Player* player : teamPlayers)
		{
			if (player != this)
			{
				Debug::DrawLine(GetPosition().x, GetPosition().y, player->GetPosition().x, player->GetPosition().y, sf::Color::White);
			}
		}
        /*const sf::Vector2f& position = GetPosition();
        Debug::DrawCircle(position.x, position.y, 10, sf::Color::Yellow); // Indicateur de balle*/
    }
}
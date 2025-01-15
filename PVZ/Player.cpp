#include "Player.h"
#include "Debug.h"
#include "Entity.h"
#include "RugbyScene.h"
#include "Utils.h"

bool Player::OpponentIsNear()
{
	return OpponentIsNear(this);
}

bool Player::OpponentIsNear(Player* player)
{
	bool isOpponentNear = false;
	RugbyScene* pScene = GetScene<RugbyScene>();
	std::vector<Player*> oppsTeamPlayers;
	pScene->GetTeamPlayers(oppsTeamPlayers, mTag == RugbyScene::Tag::PLAYER_TEAM1 ? RugbyScene::Tag::PLAYER_TEAM2 : RugbyScene::Tag::PLAYER_TEAM1);
	for (Player* oppsPlayer : oppsTeamPlayers)
	{
		float distance = Utils::GetDistance(oppsPlayer->GetPosition().x, oppsPlayer->GetPosition().y, player->GetPosition().x, player->GetPosition().y);
		if (distance <= 50.f)
		{
			isOpponentNear = true;
			break;
		}
	}
	return isOpponentNear;
}

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
				sf::Color color;
				if(OpponentIsNear(player))
					color = sf::Color::Red;
				else
					color = sf::Color::Green;
				Debug::DrawLine(GetPosition().x, GetPosition().y, player->GetPosition().x, player->GetPosition().y, color);

			}
		}
        /*const sf::Vector2f& position = GetPosition();
        Debug::DrawCircle(position.x, position.y, 10, sf::Color::Yellow); // Indicateur de balle*/
    }
}
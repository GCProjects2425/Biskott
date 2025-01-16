#include "Player.h"
#include "Debug.h"
#include "Entity.h"
#include "Utils.h"
#include "Ball.h"

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

bool Player::CheckAreaOutOfBounds()
{
	if (mArea)
	{
		const sf::Vector2f& position = GetPosition();
		sf::Vector2f newPosition = position;

		// Vérifier les limites horizontales (x)
		if ((position.x - GetRadius()) < mArea->xMin)
			newPosition.x = mArea->xMin + GetRadius();
		if ((position.x + GetRadius()) > mArea->xMax)
			newPosition.x = mArea->xMax - GetRadius();

		// Vérifier les limites verticales (y)
		if ((position.y - GetRadius()) < mArea->yMin)
			newPosition.y = mArea->yMin + GetRadius();
		if ((position.y + GetRadius()) > mArea->yMax)
			newPosition.y = mArea->yMax - GetRadius();

		// Si la position a changé, appliquer la correction
		if (newPosition != position)
		{
			SetPosition(newPosition.x, newPosition.y);
			return true; // La position a été corrigée
		}
	}
	return false; // Pas de correction nécessaire
}


void Player::MoveToPosition(float x, float y)
{
    GoToPosition(x, y, speed); // Utilise la méthode de déplacement de l'entité
}

void Player::OnUpdate()
{
	CheckAreaOutOfBounds();
	const sf::Vector2f lastPosition = GetPosition();
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


void Player::OnCollision(Entity* pCollidedWith)
{

	if (pCollidedWith->IsTag(RugbyScene::Tag::PLAYER_TEAM1) || pCollidedWith->IsTag(RugbyScene::Tag::PLAYER_TEAM2))
	{

		Player* player = dynamic_cast<Player*>(pCollidedWith);
		if (player)
		{
			if (player->HasBall())
		}
	}
}
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
		if (distance <= 100.f)
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

		if ((position.x - GetRadius()) < mArea->xMin)
			newPosition.x = mArea->xMin + GetRadius();
		if ((position.x + GetRadius()) > mArea->xMax)
			newPosition.x = mArea->xMax - GetRadius();

		if ((position.y - GetRadius()) < mArea->yMin)
			newPosition.y = mArea->yMin + GetRadius();
		if ((position.y + GetRadius()) > mArea->yMax)
			newPosition.y = mArea->yMax - GetRadius();

		if (newPosition != position)
		{
			SetPosition(newPosition.x, newPosition.y);
			return true; 
		}
	}
	return false;
}


void Player::MoveToPosition(float x, float y)
{
    GoToPosition(x, y, speed);
}

void Player::OnUpdate()
{
	RugbyScene* pScene = GetScene<RugbyScene>();

	if (HasBall())
	{
		// Récupérer le joueur le plus proche
		Player* closestTeammate = pScene->GetClosestTeammateToBall();
		if (closestTeammate)
		{
			// Tracer une ligne entre le joueur avec la balle et le plus proche
			Debug::DrawLine(
				GetPosition().x, GetPosition().y,
				closestTeammate->GetPosition().x, closestTeammate->GetPosition().y,
				sf::Color::Blue
			);
		}
	}

	CheckAreaOutOfBounds();
	const sf::Vector2f lastPosition = GetPosition();
  //  if (HasBall())
  //  {
  //      RugbyScene* pScene = GetScene<RugbyScene>();

		//std::vector<Player*> teamPlayers;
  //      pScene->GetTeamPlayers(teamPlayers, mTag);

		//for (Player* player : teamPlayers)
		//{
		//	if (player != this)
		//	{
		//		sf::Color color;
		//		if(OpponentIsNear(player))
		//			color = sf::Color::Red;
		//		else
		//			color = sf::Color::Green;
		//		Debug::DrawLine(GetPosition().x, GetPosition().y, player->GetPosition().x, player->GetPosition().y, color);

		//	}
		//}
  //      const sf::Vector2f& position = GetPosition();
  //      Debug::DrawCircle(position.x, position.y, 10, sf::Color::Yellow); // Indicateur de balle
  //  }
}


void Player::OnCollision(Entity* pCollidedWith)
{

	if (!pCollidedWith->IsTag(mTag) && !pCollidedWith->IsTag(RugbyScene::Tag::BALL))
	{
		if (Player* player = dynamic_cast<Player*>(pCollidedWith))
		{
			RugbyScene* pScene = GetScene<RugbyScene>();
			if (player->HasBall() && !pScene->GetBall()->GetIsAlreadySwitched())
			{
				Debug::DrawText(10, 10, std::to_string(GetDeltaTime()), sf::Color::Green);
				pScene->GetBall()->SetOwner(this);
				//Debug::DrawCircle(GetPosition().x, GetPosition().y, 50.f, sf::Color::Yellow);
			}

		}
	}
}
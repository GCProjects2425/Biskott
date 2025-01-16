#include "PlayerAction.h"
#include "Player.h"
#include "StateMachine.h"

#include "GameManager.h"
#include "Ball.h"
#include "RugbyScene.h"
#include "Utils.h"

void PlayerAction_Idle::OnStart(Player* pPlayer)
{
	pPlayer->SetDirection(0.f, 0.f, 0.f);
}

void PlayerAction_Attack::OnStart(Player* pPlayer)
{
	pPlayer->AddTemporaryAttribute(TemporaryAttribute::Type::Invincibility, 2.f);
	pPlayer->AddTemporaryAttribute(TemporaryAttribute::Type::IncreasedSpeed, 3.f);
	pPlayer->AddTemporaryAttribute(TemporaryAttribute::Type::PassRestriction, 2.f);
}

void PlayerAction_Attack::OnUpdate(Player* pPlayer)
{
	sf::Vector2f targetPosition = pPlayer->GetOpponentLinePosition();
	sf::Vector2f direction = targetPosition - pPlayer->GetPosition();

	float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if(magnitude > 0.f)
	{
		direction /= magnitude;
	}

	float speed = pPlayer->HasTemporaryAttribute(TemporaryAttribute::Type::IncreasedSpeed) ? PLAYER_SPEED * 1.5f : PLAYER_SPEED;
	pPlayer->SetDirection(direction.x, 0, speed);
}

void PlayerAction_Defense::OnStart(Player* pPlayer)
{

}

void PlayerAction_Defense::OnUpdate(Player* pPlayer)
{
	RugbyScene* pScene = pPlayer->GetScene<RugbyScene>();
	sf::Vector2f targetPosition = pScene->GetBall()->GetPosition();
	sf::Vector2f direction = targetPosition - pPlayer->GetPosition();

	float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (magnitude > 0.f)
	{
		direction /= magnitude;
	}

	pPlayer->SetDirection(direction.x, direction.y, PLAYER_SPEED);
}

void PlayerAction_Support::OnStart(Player* pPlayer)
{

}

void PlayerAction_Support::OnUpdate(Player* pPlayer)
{
	RugbyScene* pScene = pPlayer->GetScene<RugbyScene>();
	sf::Vector2f opponentLinePos = pPlayer->GetOpponentLinePosition();

	sf::Vector2f direction = opponentLinePos - pPlayer->GetPosition();

	float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (magnitude > 0.f)
	{
		direction /= magnitude;
	}

	if (pPlayer->IsOffside())
	{
		pPlayer->SetDirection(-direction.x, direction.y, PLAYER_SPEED);
	}
	else
	{
		pPlayer->SetDirection(direction.x, direction.y, PLAYER_SPEED);
	}
}

void PlayerAction_Passing::OnStart(Player* pPlayer)
{
	RugbyScene* pScene = pPlayer->GetScene<RugbyScene>();
	std::vector<Player*> sortedTeammates;
	pScene->GetTeamPlayers(sortedTeammates, pPlayer->IsTag(RugbyScene::PLAYER_TEAM1) ? RugbyScene::PLAYER_TEAM1 : RugbyScene::PLAYER_TEAM2);
	pPlayer->GetSortedTeammatesByDistance(sortedTeammates);

	Player* playerTarget = nullptr;

	for (std::vector<Player*>::iterator it = sortedTeammates.begin(); it != sortedTeammates.end();)
	{
		if (sortedTeammates.size() == 1)
		{
			playerTarget = *it;
			break;
		}

		if((*it)->IsOffside())
		{
			it = sortedTeammates.erase(it);
			continue;
		}

		if (pPlayer->OpponentIsInTrajectory(*it))
		{
			it = sortedTeammates.erase(it);
			continue;
		}

		if ((*it)->OpponentIsNear())
		{
			it = sortedTeammates.erase(it);
			continue;
		}

		++it;
	}

	if (sortedTeammates.size() > 0)
	{
		playerTarget = sortedTeammates[0];
	}

	if (playerTarget == nullptr)
	{
		playerTarget = pScene->GetRandomPlayerFromTeam(pPlayer->IsTag(RugbyScene::PLAYER_TEAM1) ? RugbyScene::PLAYER_TEAM1 : RugbyScene::PLAYER_TEAM2);
	}

	pScene->GetBall()->SetOwner(playerTarget);
	pScene->GetBall()->SetIsMoving(true);
}

void PlayerAction_Passing::OnUpdate(Player* pPlayer)
{
}

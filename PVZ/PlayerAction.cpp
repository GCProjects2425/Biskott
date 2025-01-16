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
	
	pPlayer->SetDirection(direction.x, 0, PLAYER_SPEED);
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
	Player* ballOwner = pScene->GetBall()->GetOwner();
	sf::Vector2f opponentLinePos = pPlayer->GetOpponentLinePosition();

	float playerDistance = Utils::GetDistance(pPlayer->GetPosition().x, pPlayer->GetPosition().y, opponentLinePos.x, opponentLinePos.y);
	float ballOwnerDistance = Utils::GetDistance(ballOwner->GetPosition().x, ballOwner->GetPosition().y, opponentLinePos.x, opponentLinePos.y);

	sf::Vector2f direction = opponentLinePos - pPlayer->GetPosition();

	float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (magnitude > 0.f)
	{
		direction /= magnitude;
	}

	if (ballOwnerDistance > playerDistance)
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
	Player* nearestTeammate = pPlayer->GetNearestTeammate();
	if (nearestTeammate)
	{
		pScene->GetBall()->SetOwner(nearestTeammate);
		pScene->GetBall()->SetIsMoving(true);
	}
}

void PlayerAction_Passing::OnUpdate(Player* pPlayer)
{
}

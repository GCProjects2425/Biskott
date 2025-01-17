#include "Player.h"
#include "Debug.h"
#include "Entity.h"
#include "Utils.h"
#include "Ball.h"

#include "StateMachine.h"
#include "PlayerAction.h"
#include "PlayerCondition.h"

#include <algorithm>
#include <cmath>

void Player::OnInitialize()
{
	mpStateMachine = new StateMachine<Player>(this, State::Count);

	//IDLE
	{
		Action<Player>* pIdle = mpStateMachine->CreateAction<PlayerAction_Idle>(State::Idle);

		//-> ATTACK
		{
			auto transition = pIdle->CreateTransition(State::Attack);
			transition->AddCondition<PlayerCondition_HasBall>();
		}

		//-> DEFENSE
		{
			auto transition = pIdle->CreateTransition(State::Defense);
			transition->AddCondition<PlayerCondition_OpponentHasBall>();
		}

		//-> SUPPORT
		{
			auto transition = pIdle->CreateTransition(State::Support);
			transition->AddCondition<PlayerCondition_HasBall>(false);
			transition->AddCondition<PlayerCondition_OpponentHasBall>(false);
		}
	}

	// ATTACK
	{
		Action<Player>* pAttack = mpStateMachine->CreateAction<PlayerAction_Attack>(State::Attack);
		//-> DEFENSE
		{
			auto transition = pAttack->CreateTransition(State::Defense);
			transition->AddCondition<PlayerCondition_HasBall>(false);
			transition->AddCondition<PlayerCondition_OpponentHasBall>();
		}

		//-> SUPPORT
		{
			auto transition = pAttack->CreateTransition(State::Support);
			transition->AddCondition<PlayerCondition_HasBall>(false);
			transition->AddCondition<PlayerCondition_OpponentHasBall>(false);
		}

		//-> PASSING
		{
			auto transition = pAttack->CreateTransition(State::Passing);
			transition->AddCondition<PlayerCondition_HasBall>(true);
			transition->AddCondition<PlayerCondition_IsInDanger>();
		}
	}

	// DEFENSE
	{
		Action<Player>* pDefense = mpStateMachine->CreateAction<PlayerAction_Defense>(State::Defense);
		//-> ATTACK
		{
			auto transition = pDefense->CreateTransition(State::Attack);
			transition->AddCondition<PlayerCondition_HasBall>();
		}

		//-> SUPPORT
		{
			auto transition = pDefense->CreateTransition(State::Support);
			transition->AddCondition<PlayerCondition_HasBall>(false);
			transition->AddCondition<PlayerCondition_OpponentHasBall>(false);
		}
	}

	// SUPPORT
	{
		Action<Player>* pSupport = mpStateMachine->CreateAction<PlayerAction_Support>(State::Support);
		//-> ATTACK
		{
			auto transition = pSupport->CreateTransition(State::Attack);
			transition->AddCondition<PlayerCondition_HasBall>();
		}

		//-> DEFENSE
		{
			auto transition = pSupport->CreateTransition(State::Defense);
			transition->AddCondition<PlayerCondition_OpponentHasBall>();
		}
	}

	// PASSING
	{
		Action<Player>* pPassing = mpStateMachine->CreateAction<PlayerAction_Passing>(State::Passing);
		//-> ATTACK
		{
			auto transition = pPassing->CreateTransition(State::Attack);
			transition->AddCondition<PlayerCondition_HasBall>(false);
		}
	}

	mpStateMachine->SetState(State::Idle);
}

bool Player::IsScoringATry() const
{
	float distance = Utils::GetDistance(GetPosition().x, GetPosition().y, GetOpponentLinePosition().x, GetOpponentLinePosition().y);
	return HasBall() && distance <= 5;
}

Player* Player::OpponentIsNear()
{
	return OpponentIsNear(this);
}

Player* Player::OpponentIsNear(Player* player)
{
	RugbyScene* pScene = GetScene<RugbyScene>();
	std::vector<Player*> oppsTeamPlayers;
	pScene->GetTeamPlayers(oppsTeamPlayers, mTag == RugbyScene::Tag::PLAYER_TEAM1 ? RugbyScene::Tag::PLAYER_TEAM2 : RugbyScene::Tag::PLAYER_TEAM1);
	for (Player* oppsPlayer : oppsTeamPlayers)
	{
		float distance = Utils::GetDistance(oppsPlayer->GetPosition().x, oppsPlayer->GetPosition().y, player->GetPosition().x, player->GetPosition().y);
		if (distance <= 50.f)
		{
			return oppsPlayer;
		}
	}
	return nullptr;
}

Player* Player::OpponentIsInTrajectory(Player* playerTarget)
{
	return OpponentIsInTrajectory(this, playerTarget);
}

Player* Player::OpponentIsInTrajectory(Player* player, Player* playerTarget)
{
	RugbyScene* pScene = GetScene<RugbyScene>();
	std::vector<Player*> oppsTeamPlayers;
	pScene->GetTeamPlayers(oppsTeamPlayers, mTag == RugbyScene::Tag::PLAYER_TEAM1 ? RugbyScene::Tag::PLAYER_TEAM2 : RugbyScene::Tag::PLAYER_TEAM1);

	Player* nearestPlayer = nullptr;
	float minDistance = std::numeric_limits<float>::max();

	for (Player* oppsPlayer : oppsTeamPlayers)
	{
		sf::Vector2f AB = playerTarget->GetPosition() - player->GetPosition();
		sf::Vector2f AC = oppsPlayer->GetPosition() - player->GetPosition();
		sf::Vector2f BC = oppsPlayer->GetPosition() - playerTarget->GetPosition();

		float dotAC_AB = AC.x * AB.x + AC.y * AB.y;
		float dotBC_AB = BC.x * AB.x + BC.y * AB.y;

		bool isBetween = dotAC_AB >= 0 && dotBC_AB <= 0;

		if(isBetween)
		{
			float distance = std::abs(AB.y * oppsPlayer->GetPosition().x
				- AB.x * oppsPlayer->GetPosition().y
				+ playerTarget->GetPosition().x * player->GetPosition().y
				- playerTarget->GetPosition().y * player->GetPosition().x)
				/ std::sqrt(AB.y * AB.y + AB.x * AB.x);

			if (distance <= oppsPlayer->GetRadius() && distance < minDistance)
			{
				minDistance = distance;
				nearestPlayer = oppsPlayer;
			}
		}
	}
	return nearestPlayer;
}

Player* Player::GetNearestPlayerToOpponentLine()
{
	RugbyScene* pScene = GetScene<RugbyScene>();
	std::vector<Player*> teammates;
	pScene->GetTeamPlayers(teammates, mTag);

	float opponentLineX = GetOpponentLinePosition().x;

	Player* nearestPlayer = nullptr;
	float minDistance = std::numeric_limits<float>::max();

	for (Player* teammate : teammates)
	{
		float distance = std::abs(opponentLineX - teammate->GetPosition().x);

		if (distance < minDistance)
		{
			minDistance = distance;
			nearestPlayer = teammate;
		}
	}
	return nearestPlayer;
}

const sf::Vector2f& Player::GetOpponentLinePosition() const
{
	RugbyScene* pScene = GetScene<RugbyScene>();

	sf::Vector2f position;
	position.x = pScene->GetWindowWidth() * (mTag == RugbyScene::Tag::PLAYER_TEAM1 ? 0.9f : 0.1f);
	position.y = GetPosition().y;
	return position;
}

bool Player::IsOffside()
{
	Player* ballOwner = GetScene<RugbyScene>()->GetBall()->GetOwner();
	if (ballOwner == this || !ballOwner->IsTag(mTag))
	{
		return false;
	}

	float playerDistance = Utils::GetDistance(GetPosition().x, GetPosition().y, GetOpponentLinePosition().x, GetOpponentLinePosition().y);
	float ballOwnerDistance = Utils::GetDistance(ballOwner->GetPosition().x, ballOwner->GetPosition().y, GetOpponentLinePosition().x, GetOpponentLinePosition().y);

	return ballOwnerDistance > playerDistance;
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

Player* Player::GetNearestTeammate()
{
	RugbyScene* pScene = GetScene<RugbyScene>();
	std::vector<Player*> teammates;
	pScene->GetTeamPlayers(teammates, mTag);

	Player* closestPlayer = nullptr;
	float minDistance = std::numeric_limits<float>::max();

	for (Player* teammate : teammates)
	{
		if (teammate == this)
			continue;

		sf::Vector2f playerPosition = GetPosition();
		sf::Vector2f teammatePosition = teammate->GetPosition();

		// Calculer la distance
		float distance = Utils::GetDistance(
			static_cast<int>(playerPosition.x), static_cast<int>(playerPosition.y),
			static_cast<int>(teammatePosition.x), static_cast<int>(teammatePosition.y)
		);

		if (distance < minDistance)
		{
			minDistance = distance;
			closestPlayer = teammate;
		}
	}
	return closestPlayer;
}

void Player::GetSortedTeammatesByDistance(std::vector<Player*>& sortedTeammates)
{
	std::sort(sortedTeammates.begin(), sortedTeammates.end(), [&](Player* a, Player* b)
		{
			return Utils::GetDistance(GetPosition().x, GetPosition().y, a->GetPosition().x, a->GetPosition().y) <
				Utils::GetDistance(GetPosition().x, GetPosition().y, b->GetPosition().x, b->GetPosition().y);
		});
}

const char* Player::GetStateName(State state) const
{
	switch (state)
	{
	case Idle: return "Idle";
	case Attack: return "Attack";
	case Defense: return "Defense";
	case Support: return "Support";
	case Passing: return "Passing";
	default: return "Unknown";
	}
}

void Player::Dodge()
{
	if (Player* nearestPlayer = OpponentIsNear())
	{
		sf::Vector2f targetDirection = GetPosition() - GetOpponentLinePosition();
		float magnitude = std::sqrt(targetDirection.x * targetDirection.x + targetDirection.y * targetDirection.y);
		if (magnitude > 0.f)
		{
			targetDirection /= magnitude;
		}
		sf::Vector2f dodgeDirection = GetPosition() - nearestPlayer->GetPosition();
		magnitude = std::sqrt(dodgeDirection.x * dodgeDirection.x + dodgeDirection.y * dodgeDirection.y);
		if (magnitude > 0.f)
		{
			dodgeDirection /= magnitude;
		}
		sf::Vector2f finalDirection = targetDirection + dodgeDirection * 0.5f;
		float finalMagnitude = std::sqrt(finalDirection.x * finalDirection.x + finalDirection.y * finalDirection.y);
		if (finalMagnitude > 0.f)
		{
			finalDirection /= finalMagnitude;
		}
		float dodgeSpeed = 150.f;
		SetDirection(finalDirection.x, finalDirection.y, dodgeSpeed);
	}
}


void Player::MoveToPosition(float x, float y)
{
    GoToPosition(x, y, speed);
}

void Player::OnUpdate()
{
	UpdateTemporaryAttributes(GetDeltaTime());
	RugbyScene* pScene = GetScene<RugbyScene>();

	if (HasBall())
	{
		std::vector<Player*> teamPlayers;
		pScene->GetTeamPlayers(teamPlayers, mTag);
		for (Player* player : teamPlayers)
		{
			if (player != this)
			{
				sf::Color color;
				if (OpponentIsNear(player) || OpponentIsInTrajectory(player))
					color = sf::Color::Red;
				else
					color = sf::Color::Green;
				Debug::DrawLine(GetPosition().x, GetPosition().y, player->GetPosition().x, player->GetPosition().y, color);

			}
		}

		// Recupere le joueur le plus proche
		Player* closestTeammate = GetNearestTeammate();
		if (closestTeammate)
		{
			// Trace une ligne entre le joueur avec la balle et le plus proche
			Debug::DrawLine(
				GetPosition().x, GetPosition().y,
				closestTeammate->GetPosition().x, closestTeammate->GetPosition().y,
				sf::Color::Blue
			);
		}
	}

	const char* stateName = GetStateName((State)mpStateMachine->GetCurrentState());
	Debug::DrawText(GetPosition().x - GetRadius(), GetPosition().y - GetRadius(), stateName, sf::Color::White);

	CheckAreaOutOfBounds();
	mpStateMachine->Update();
}


void Player::OnCollision(Entity* pCollidedWith)
{
	if (!pCollidedWith->IsTag(mTag) && !pCollidedWith->IsTag(RugbyScene::Tag::BALL) && !HasTemporaryAttribute(TemporaryAttribute::Type::Invincibility))
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

void Player::AddTemporaryAttribute(TemporaryAttribute::Type type, float duration)
{
	mTemporaryAttributes.push_back({type, duration});
}

void Player::UpdateTemporaryAttributes(float deltaTime)
{
	for (auto it = mTemporaryAttributes.begin(); it != mTemporaryAttributes.end();)
	{
		it->duration -= deltaTime;
		if (it->duration <= 0.f)
		{
			it = mTemporaryAttributes.erase(it); // Supprime l'attribut expiré
		}
		else
		{
			++it;
		}
	}
}

bool Player::HasTemporaryAttribute(TemporaryAttribute::Type type) const
{
	for (const auto& attr : mTemporaryAttributes)
	{
		if (attr.type == type)
			return true;
	}
	return false;
}

#include "Player.h"
#include "Debug.h"
#include "Entity.h"
#include "Utils.h"
#include "Ball.h"

#include "StateMachine.h"
#include "PlayerAction.h"
#include "PlayerCondition.h"

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
		{
			auto transition = pPassing->CreateTransition(State::Attack);
			transition->AddCondition<PlayerCondition_HasBall>(false);
		}
	}

	//SHOOTING
	/*{
		Action<Plant>* pShooting = mpStateMachine->CreateAction<PlantAction_Shooting>(State::Shooting);

		//-> IDLE
		{
			auto transition = pShooting->CreateTransition(State::Idle);

			transition->AddCondition<PlantCondition_ZombieOnLane>(false);
		}

		//-> RELOADING
		{
			auto transition = pShooting->CreateTransition(State::Reloading);

			transition->AddCondition<PlantCondition_NoAmmo>();
		}
	}

	//RELOADING
	{
		Action<Plant>* pShooting = mpStateMachine->CreateAction<PlantAction_Reloading>(State::Reloading);
	}*/

	mpStateMachine->SetState(State::Idle);
}

bool Player::OpponentIsNear()
{
	return OpponentIsNear(this);
}

bool Player::OpponentIsNear(Player* player)
{
	RugbyScene* pScene = GetScene<RugbyScene>();
	std::vector<Player*> oppsTeamPlayers;
	pScene->GetTeamPlayers(oppsTeamPlayers, mTag == RugbyScene::Tag::PLAYER_TEAM1 ? RugbyScene::Tag::PLAYER_TEAM2 : RugbyScene::Tag::PLAYER_TEAM1);
	for (Player* oppsPlayer : oppsTeamPlayers)
	{
		float distance = Utils::GetDistance(oppsPlayer->GetPosition().x, oppsPlayer->GetPosition().y, player->GetPosition().x, player->GetPosition().y);
		if (distance <= 50.f)
		{
			return true;
		}
	}
	return false;
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


void Player::MoveToPosition(float x, float y)
{
    GoToPosition(x, y, speed);
}

void Player::OnUpdate()
{
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

		// R�cup�rer le joueur le plus proche
		Player* closestTeammate = GetNearestTeammate();
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

	if (GetNearestPlayerToOpponentLine() == this)
	{
		Debug::DrawText(GetPosition().x, GetPosition().y, "top", sf::Color::Green);
	}

	CheckAreaOutOfBounds();
	mpStateMachine->Update();
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
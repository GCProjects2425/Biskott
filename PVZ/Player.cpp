#include "Player.h"

#include <string>

#include "PVZScene.h"
#include "Ball.h"

#include "StateMachine.h"
#include "PlayerAction.h"
#include "PlayerCondition.h"

#include "Debug.h"

void Player::OnInitialize()
{
	mpStateMachine = new StateMachine<Player>(this, State::Count);

	mAreaIndex = -1;
	mAmmo = mMaxAmmo;

	//IDLE
	{
		Action<Player>* pIdle = mpStateMachine->CreateAction<PlayerAction_Idle>(State::Idle);

		//-> SHOOTING
		{
			auto transition = pIdle->CreateTransition(State::Attack);

			auto condition = transition->AddCondition<PlantCondition_ZombieOnLane>();
		}

		//-> RELOADING
		/*{
			auto transition = pIdle->CreateTransition(State::Reloading);

			transition->AddCondition<PlantCondition_FullAmmo>(false);
			transition->AddCondition<PlantCondition_ZombieOnLane>(false);
		}*/
	}

	mpStateMachine->SetState(State::Idle);
}

const char* Player::GetStateName(State state) const
{
	switch (state)
	{
	case Idle: return "Idle";
	case Defense: return "Defense";
	case Attack: return "Attack";
	case Support: return "Support";
	default: return "Unknown";
	}
}

void Player::Shoot()
{
	if (mAmmo <= 0)
		return;

	const sf::Vector2f& position = GetPosition();

	Ball* pProjectile = CreateEntity<Ball>(5.0f, sf::Color::Red);
	pProjectile->SetPosition(position.x, position.y);
	
	mAmmo--;
}

void Player::Reload()
{
	mAmmo = mMaxAmmo;
}

void Player::OnUpdate()
{
	const sf::Vector2f& position = GetPosition();
	const char* stateName = GetStateName((Player::State)mpStateMachine->GetCurrentState());

	std::string ammo = std::to_string(mAmmo) + "/" + std::to_string(mMaxAmmo);

	//Debug::DrawText(position.x, position.y - 50, stateName, 0.5f, 0.5f, sf::Color::Red);
	//Debug::DrawText(position.x, position.y, ammo, 0.5f, 0.5f, sf::Color::Blue);

	mpStateMachine->Update();
}

#pragma once

#include "Entity.h"

template<typename T>
class StateMachine;

class Player : public Entity
{
	StateMachine<Player>* mpStateMachine;

	enum State 
	{
		Idle,
		Defense,
		Attack,
		Support,

		Count
	};

	int mAmmo;
	int mMaxAmmo = 6;

	float mShootCadence = 1.0f;
	
	float mReloadDuration = 2.0f;
	
	int mAreaIndex;

public:
	void SetAreaIndex(int index) { mAreaIndex = index; }
	const char* GetStateName(State state) const;

	void Shoot();
	void Reload();
	
protected:
	void OnInitialize() override;
	void OnUpdate() override;

	friend class PlayerAction_Idle;
	friend class PlayerAction_Defense;
	friend class PlayerAction_Attack;

	friend class PlantCondition_FullAmmo;
	friend class PlantCondition_NoAmmo;
	friend class PlantCondition_ZombieOnLane;
};

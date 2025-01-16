#pragma once
#include "Entity.h"
#include "RugbyScene.h"

template<typename T>
class StateMachine;

class Player : public Entity
{
    StateMachine<Player>* mpStateMachine;

    enum State
    {
        Idle,
        Attack,
        Defense,
        Support,

        Count
    };

    bool hasBall = false;
    float speed = 100.0f;
	AABB* mArea = nullptr;
public:
    void SetHasBall(bool value) { hasBall = value; }
    bool HasBall() const { return hasBall; }

    void SetArea(AABB* area) { mArea = area; };

    void OnInitialize();

    bool OpponentIsNear();
    bool OpponentIsNear(Player* player);
	Player* OpponentIsInTrajectory(Player* playerTarget);
	Player* OpponentIsInTrajectory(Player* player, Player* playerTarget);
    Player* GetNearestPlayerToOpponentLine();
	const sf::Vector2f& GetOpponentLinePosition() const;

    bool CheckAreaOutOfBounds();

    Player* GetNearestTeammate();

    void MoveToPosition(float x, float y);
    void OnUpdate();
    void OnCollision(Entity* pCollidedWith);

    friend class PlayerAction_Idle;
	friend class PlayerAction_Attack;
	friend class PlayerAction_Defense;
	friend class PlayerAction_Support;

	friend class PlayerCondition_HasBall;
    friend class PlayerCondition_OpponentHasBall;
};
#pragma once
#include "Entity.h"
#include "RugbyScene.h"

template<typename T>
class StateMachine;

struct TemporaryAttribute
{
    enum class Type
    {
        Invincibility,
        IncreasedSpeed,
        PassRestriction
    };

    Type type;
    float duration; // Temps restant en secondes
};

struct PlayerStats
{
    float speed;
    float passPower;
    float sprintDuration;
    float sprintSpeed;
};

class Player : public Entity
{
    StateMachine<Player>* mpStateMachine;

    enum State
    {
        Idle,
        Attack,
        Defense,
        Support,
        Passing,

        Count
    };

    bool hasBall = false;
	AABB* mArea = nullptr;

    std::vector<TemporaryAttribute> mTemporaryAttributes;
	PlayerStats mStats;
public:
    void SetHasBall(bool value) { hasBall = value; }
    bool HasBall() const { return hasBall; }

    void SetArea(AABB* area) { mArea = area; };

    void OnInitialize();

	const PlayerStats& GetStats() const { return mStats; }

    Player* OpponentIsNear();
    Player* OpponentIsNear(Player* player);
	Player* OpponentIsInTrajectory(Player* playerTarget);
	Player* OpponentIsInTrajectory(Player* player, Player* playerTarget);
    Player* GetNearestPlayerToOpponentLine();
	const sf::Vector2f& GetOpponentLinePosition() const;

    bool IsOffside();

    bool CheckAreaOutOfBounds();

    Player* GetNearestTeammate();
	void GetSortedTeammatesByDistance(std::vector<Player*>& sortedTeammates);

	const char* GetStateName(State state) const;

	void Dodge();

    void MoveToPosition(float x, float y);
    void OnUpdate();
    void OnCollision(Entity* pCollidedWith);

    void RandomizeStats();

    void AddTemporaryAttribute(TemporaryAttribute::Type type, float duration);
    void UpdateTemporaryAttributes(float deltaTime);
    bool HasTemporaryAttribute(TemporaryAttribute::Type type) const;

    friend class PlayerAction_Idle;
	friend class PlayerAction_Attack;
	friend class PlayerAction_Defense;
	friend class PlayerAction_Support;
	friend class PlayerAction_Passing;

	friend class PlayerCondition_HasBall;
    friend class PlayerCondition_OpponentHasBall;
    friend class PlayerCondition_IsInDanger;
};
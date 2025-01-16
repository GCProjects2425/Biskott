#pragma once
#include "Entity.h"
#include "RugbyScene.h"

class Player : public Entity
{
private:
    bool hasBall = false;
    float speed = 100.0f;
	AABB* mArea = nullptr;
public:
    void SetHasBall(bool value) { hasBall = value; }
    bool HasBall() const { return hasBall; }

    void SetArea(AABB* area) { mArea = area; };

    bool OpponentIsNear();
    bool OpponentIsNear(Player* player);

    bool CheckAreaOutOfBounds();

    void MoveToPosition(float x, float y);
    void OnUpdate();
    void OnCollision(Entity* pCollidedWith);
};
#pragma once
#include "Entity.h"
#include "Player.h"

class Ball : public Entity
{
public:
	void SetOwner(Player* owner); 
	Player* GetOwner() const { return mOwner; } 

	void SetIsMoving(bool value) { mIsMoving = value; }
	bool IsMoving() const { return mIsMoving; }

	void SetIsAlreadySwitched(bool value) { isAlreadySwitched = value; }
	bool GetIsAlreadySwitched() const { return isAlreadySwitched; }

    void OnUpdate(); 
private:
	Player* mOwner = nullptr;
	bool mIsMoving = false;
	bool isAlreadySwitched = false;
};
#pragma once
#include "Entity.h"
#include "Player.h"

class Ball : public Entity
{
public:
	void SetOwner(Player* owner); // D�finir le joueur poss�dant la balle
	Player* GetOwner() const { return mOwner; } // Obtenir le joueur poss�dant la balle

	void SetIsMoving(bool value) { mIsMoving = value; } // D�finir si la balle est en mouvement
	bool IsMoving() const { return mIsMoving; } // Indique si la balle est en mouvement

    void OnUpdate(); // D�placement de la balle
private:
	Player* mOwner = nullptr; // Joueur poss�dant la balle
	bool mIsMoving = false; // Indique si la balle est en mouvement
};
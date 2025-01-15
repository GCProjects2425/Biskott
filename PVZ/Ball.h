#pragma once
#include "Entity.h"
#include "Player.h"

class Ball : public Entity
{
public:
	void SetOwner(Player* owner); // Définir le joueur possédant la balle
	Player* GetOwner() const { return mOwner; } // Obtenir le joueur possédant la balle

	void SetIsMoving(bool value) { mIsMoving = value; } // Définir si la balle est en mouvement
	bool IsMoving() const { return mIsMoving; } // Indique si la balle est en mouvement

    void OnUpdate(); // Déplacement de la balle
private:
	Player* mOwner = nullptr; // Joueur possédant la balle
	bool mIsMoving = false; // Indique si la balle est en mouvement
};
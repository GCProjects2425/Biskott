#pragma once
#include "Entity.h"
#include "Player.h"

class Ball : public Entity
{
public:
	void SetOwner(Player* owner) { mOwner = owner; }; // D�finir le joueur poss�dant la balle

    void OnUpdate(); // D�placement de la balle
private:
	Player* mOwner = nullptr; // Joueur poss�dant la balle
};
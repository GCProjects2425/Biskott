#pragma once
#include "Entity.h"
#include "Player.h"

class Ball : public Entity
{
public:
	void SetOwner(Player* owner) { mOwner = owner; }; // Définir le joueur possédant la balle

    void OnUpdate(); // Déplacement de la balle
private:
	Player* mOwner = nullptr; // Joueur possédant la balle
};
#pragma once
#include "Entity.h"

class Player : public Entity
{
private:
    bool hasBall = false; // Indique si le joueur possède la balle
    float speed = 100.0f; // Vitesse du joueur

public:
    void SetHasBall(bool value) { hasBall = value; }
    bool HasBall() const { return hasBall; }

    void MoveToPosition(float x, float y); // Se déplacer vers une position
    void OnUpdate(); // Comportement du joueur
};
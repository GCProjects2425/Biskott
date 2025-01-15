#pragma once
#include "Scene.h"
#include <vector>

class Player;
class Ball;

class RugbyScene : public Scene
{
private:
    std::vector<Player*> mTeam1; // Joueurs de l'�quipe 1
    std::vector<Player*> mTeam2; // Joueurs de l'�quipe 2
    Ball* mBall; // Instance de la balle
    int mScoreTeam1 = 0; // Score de l'�quipe 1
    int mScoreTeam2 = 0; // Score de l'�quipe 2

public:
    enum Tag
    {
        PLAYER_TEAM1,
        PLAYER_TEAM2,
        BALL
    };

    void OnInitialize() override;
    void OnEvent(const sf::Event& event) override;
    void OnUpdate() override;

private:
    void InitializeTeams(); // Initialiser les joueurs
    void ResetPositions();  // R�initialiser les positions apr�s un essai
    void UpdateScore(int team); // Mettre � jour le score
    void DrawField(); // Dessiner le terrain
};
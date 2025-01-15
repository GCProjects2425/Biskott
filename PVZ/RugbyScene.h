#pragma once
#include "Scene.h"
#include <vector>

class Player;
class Ball;

class RugbyScene : public Scene
{
private:
    std::vector<Player*> mTeam1;
    std::vector<Player*> mTeam2;
    Ball* mBall;
    int mScoreTeam1 = 0; 
    int mScoreTeam2 = 0; 

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
    void InitializeTeams(); 
    void ResetPositions(); 
    void UpdateScore(int team);
    void DrawField();
    Player* mSelectedPlayer = nullptr;
};
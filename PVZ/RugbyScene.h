#pragma once
#include "Scene.h"
#include <vector>
#include "AABB.h"

#define PLAYER_SPEED 50.f

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

    AABB mAreas[3];

    Player* mSelectedPlayer = nullptr;

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

	void GetTeamPlayers(std::vector<Player*>& team, int teamIndex) const;
    const int GetTeamWithBall() const;
	Ball* GetBall() const { return mBall; }

private:
    void InitializeTeams(); 
    void ResetPositions(); 
    void UpdateScore(int team);
    void DrawField();
    
    int GetPlayerLane(const sf::Vector2f& position) const; 
};
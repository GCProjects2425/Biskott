#pragma once
#include "Scene.h"
#include <vector>
#include "AABB.h"

#define PLAYER_SPEED 100.f

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
    bool isPaused = false;
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

	bool IsPaused() const { return isPaused; }

	void GetTeamPlayers(std::vector<Player*>& team, int teamIndex) const;
    const int GetTeamWithBall() const;
	Ball* GetBall() const { return mBall; }

	Player* GetRandomPlayerFromTeam(int teamIndex) const;

	int CheckIfScoringATry() const;

private:
    void InitializeTeams(); 
    void ResetPositions(int teamWin); 
    void UpdateScore(int team);
    void DrawField();
    
    int GetPlayerLane(const sf::Vector2f& position) const;
    void DrawScore();
};
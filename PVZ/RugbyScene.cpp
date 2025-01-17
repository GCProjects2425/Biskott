#include "RugbyScene.h"
#include "Player.h"
#include "Ball.h"
#include "Debug.h"
#include "GameManager.h"
#include "Utils.h"

#include <random>

#define LANE_COUNT 3

void RugbyScene::OnInitialize()
{
    int width = GetWindowWidth();
    int height = GetWindowHeight();

    int halfHeight = height / 2;
    int quarterHeight = height / 4;

    // Zone Rouge : Haut jusqu'à la moitié
    mAreas[0] = { 1, 0, width - 1, halfHeight };

    // Zone Bleue : Moitié jusqu'au bas
    mAreas[1] = { 1, halfHeight, width - 1, height };

    // Zone Jaune : Centre (chevauche Rouge et Bleue)
    mAreas[2] = { 1, quarterHeight, width - 1, 3 * quarterHeight };

    InitializeTeams();

    mBall = CreateEntity<Ball>(10.0f, sf::Color::Magenta);
    mBall->SetPosition(width / 2, height / 2);
    mBall->SetTag(BALL);
    mBall->SetOwner(GetRandomPlayerFromTeam(PLAYER_TEAM1));
}

void RugbyScene::OnEvent(const sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        mSelectedPlayer = nullptr; 
        for (Player* player : mTeam1)
        {
            if (player->IsInside(event.mouseButton.x, event.mouseButton.y))
            {
                mSelectedPlayer = player;
                return;
            }
        }

        for (Player* player : mTeam2)
        {
            if (player->IsInside(event.mouseButton.x, event.mouseButton.y))
            {
                mSelectedPlayer = player;
                return;
            }
        }
    }
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        if (mSelectedPlayer)
        {
            mSelectedPlayer = nullptr;
        }
    }

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
	{
		if (mBall->GetOwner() && mBall->GetOwner()->IsTag(PLAYER_TEAM1))
		{
            for (Player* player : mTeam1)
            {
                if (player->IsInside(event.mouseButton.x, event.mouseButton.y))
                {

                    if(Player* interceptor = mBall->GetOwner()->OpponentIsInTrajectory(player))
					{
						mBall->SetOwner(interceptor);
					}
                    else
                    {
						mBall->SetOwner(player);
                    }
					mBall->SetIsMoving(true);
                    return;
                }
            }
		}
		else if (mBall->GetOwner() && mBall->GetOwner()->IsTag(PLAYER_TEAM2))
        {
            for (Player* player : mTeam2)
            {
                if (player->IsInside(event.mouseButton.x, event.mouseButton.y))
                {
                    if (Player* interceptor = mBall->GetOwner()->OpponentIsInTrajectory(player))
                    {
                        mBall->SetOwner(interceptor);
                    }
                    else
                    {
                        mBall->SetOwner(player);
                    }
					mBall->SetIsMoving(true);
                    return;
                }
            }
        }
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
	{
        isPaused = !isPaused;
	}

    if(event.type == sf::Event::MouseMoved)
    {
		if (mSelectedPlayer)
		{
			mSelectedPlayer->SetPosition(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
		}
    }
}


void RugbyScene::OnUpdate()
{
    // Dessiner le terrain
    DrawField();

    // Dessiner le score 
    DrawScore();

    // Dessiner les areas 
    for (int i = 0; i < LANE_COUNT; i++)
    {
        const AABB& aabb = mAreas[i];
        sf::Color color;
        if (i == 0) color = sf::Color::Red;     // Rouge pour la première zone
        else if (i == 1) color = sf::Color::Blue; // Bleu pour la deuxième zone
        else if (i == 2) color = sf::Color::Yellow; // Jaune pour la troisième zone

        Debug::DrawRectangle(aabb.xMin, aabb.yMin, aabb.xMax - aabb.xMin, aabb.yMax - aabb.yMin, color);
    }

    int team = CheckIfScoringATry();
	if (team >= 0)
	{
		UpdateScore(team);
	}
}

void RugbyScene::GetTeamPlayers(std::vector<Player*>& team, int teamIndex) const
{
	if (teamIndex == static_cast<int>(PLAYER_TEAM1))
	{
		team = mTeam1;
	}
	else if (teamIndex == static_cast<int>(PLAYER_TEAM2))
	{
		team = mTeam2;
	}
}

const int RugbyScene::GetTeamWithBall() const
{
    return GetBall()->GetOwner()->IsTag(PLAYER_TEAM1) ? PLAYER_TEAM1 : PLAYER_TEAM2;
}

Player* RugbyScene::GetRandomPlayerFromTeam(int teamIndex) const
{
	int randomIndex = Utils::Random(0, 4);

	if (teamIndex == PLAYER_TEAM1)
	{
		return mTeam1[randomIndex];
	}
	else if (teamIndex == PLAYER_TEAM2)
	{
		return mTeam2[randomIndex];
	}
    return nullptr;
}

int RugbyScene::CheckIfScoringATry() const
{
    int teamIdx = GetBall()->IsScoringATry();
    if(teamIdx >= 0)
	{
		return teamIdx;
	}
    return -1;
}

void RugbyScene::InitializeTeams()
{
    int width = GetWindowWidth();
    int height = GetWindowHeight();
    float playerRadius = 20.0f;

    for (int i = 0; i < 5; ++i)
    {
        Player* player = CreateEntity<Player>(playerRadius, sf::Color::Green);
        player->SetPosition(width * 0.1f, (i + 1) * height / 6);
		player->SetTag(PLAYER_TEAM1);
        player->SetRigidBody(true);
		player->RandomizeStats();

        if (i <= 1)
            player->SetArea(&mAreas[0]);
        else if (i >= 3)
            player->SetArea(&mAreas[1]);
        else
            player->SetArea(&mAreas[2]);

        mTeam1.push_back(player);
    }

    for (int i = 0; i < 5; ++i)
    {
        Player* player = CreateEntity<Player>(playerRadius, sf::Color::Red);
        player->SetPosition(width * 0.9f, (i + 1) * height / 6);
        player->SetTag(PLAYER_TEAM2);
        player->SetRigidBody(true);
		player->RandomizeStats();

        if (i <= 1)
            player->SetArea(&mAreas[0]);
        else if (i >= 3)
            player->SetArea(&mAreas[1]);
        else
            player->SetArea(&mAreas[2]);

        mTeam2.push_back(player);
    }
}

void RugbyScene::ResetPositions(int teamWin)
{
    int width = GetWindowWidth();
    int height = GetWindowHeight();

    for (int i = 0; i < mTeam1.size(); ++i)
    {
        mTeam1[i]->SetPosition(width * 0.1f, (i + 1) * height / 6);
        mTeam1[i]->RandomizeStats();
    }

    for (int i = 0; i < mTeam2.size(); ++i)
    {
        mTeam2[i]->SetPosition(width * 0.9f, (i + 1) * height / 6);
		mTeam2[i]->RandomizeStats();
    }

    // Positionnenement de la balle au centre
    mBall->SetOwner(GetRandomPlayerFromTeam(teamWin == PLAYER_TEAM1 ? PLAYER_TEAM2 : PLAYER_TEAM1));
}

void RugbyScene::UpdateScore(int team)
{
    if (team == PLAYER_TEAM1)
    {
        mScoreTeam1++;
    }
    else if (team == PLAYER_TEAM2)
    {
        mScoreTeam2++;
    }

    ResetPositions(team);
}

void RugbyScene::DrawField()
{
    int width = GetWindowWidth();
    int height = GetWindowHeight();

    // Fond bleu
    Debug::DrawRectangle(0, 0, width, height, sf::Color::Blue);

    // Lignes blanches d�limitant les zones
    Debug::DrawLine(width * 0.1f, 0, width * 0.1f, height, sf::Color::White);
    Debug::DrawLine(width * 0.9f, 0, width * 0.9f, height, sf::Color::White);
}

int RugbyScene::GetPlayerLane(const sf::Vector2f& position) const
{
    for (int i = 0; i < LANE_COUNT; i++)
    {
        const AABB& aabb = mAreas[i];
        if (position.x >= aabb.xMin && position.x <= aabb.xMax &&
            position.y >= aabb.yMin && position.y <= aabb.yMax)
        {
            return i; // Retourne l'index de la zone
        }
    }
    return -1;
}

void RugbyScene::DrawScore()
{
    int width = GetWindowWidth();

    std::string scoreText =  std::to_string(mScoreTeam1) + " - " + std::to_string(mScoreTeam2);
    float xPosition = width / 2.0f - 50;
    float yPosition = 20.0f;  

    // Dessiner le texte
    Debug::DrawText(xPosition, yPosition, scoreText, sf::Color::White);
}
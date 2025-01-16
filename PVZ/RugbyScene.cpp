#include "RugbyScene.h"
#include "Player.h"
#include "Ball.h"
#include "Debug.h"
#include "GameManager.h"

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

    // Initialiser les équipes
    InitializeTeams();

    // Créer la balle
    mBall = CreateEntity<Ball>(10.0f, sf::Color::Magenta);
    mBall->SetPosition(width / 2, height / 2);
    mBall->SetTag(BALL);
    mBall->SetOwner(mTeam1[1]);
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
                    mBall->SetOwner(player);
					mBall->SetIsMoving(true);
                    return;
                }
            }
		}
        else
        {
            for (Player* player : mTeam2)
            {
                if (player->IsInside(event.mouseButton.x, event.mouseButton.y))
                {
                    mBall->SetOwner(player);
                    return;
                }
            }
        }
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

    // Dessiner les zones 
    for (int i = 0; i < LANE_COUNT; i++)
    {
        const AABB& aabb = mAreas[i];
        sf::Color color;
        if (i == 0) color = sf::Color::Red;     // Rouge pour la première zone
        else if (i == 1) color = sf::Color::Blue; // Bleu pour la deuxième zone
        else if (i == 2) color = sf::Color::Yellow; // Jaune pour la troisième zone

        //Debug::DrawRectangle(aabb.xMin, aabb.yMin, aabb.xMax, aabb.yMax, color);
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

        if (i <= 1)
            player->SetArea(&mAreas[0]);
        else if (i >= 3)
            player->SetArea(&mAreas[1]);
        else
            player->SetArea(&mAreas[2]);

        mTeam2.push_back(player);
    }
}

void RugbyScene::ResetPositions()
{
    // R�initialiser les positions des joueurs et de la balle apr�s un essai
    int width = GetWindowWidth();
    int height = GetWindowHeight();

    for (int i = 0; i < mTeam1.size(); ++i)
    {
        mTeam1[i]->SetPosition(width * 0.1f, (i + 1) * height / 6);
    }

    for (int i = 0; i < mTeam2.size(); ++i)
    {
        mTeam2[i]->SetPosition(width * 0.9f, (i + 1) * height / 6);
    }

    // Positionner la balle au centre
    mBall->SetPosition(width / 2, height / 2);
}

void RugbyScene::UpdateScore(int team)
{
    if (team == 1)
    {
        mScoreTeam1++;
    }
    else if (team == 2)
    {
        mScoreTeam2++;
    }

    // R�initialiser les positions apr�s la mise � jour du score
    ResetPositions();
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
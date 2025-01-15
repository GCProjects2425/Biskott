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
    int zoneHeight = height / LANE_COUNT;

    mAreas[0] = { 0, 0, width, zoneHeight };                    
    mAreas[1] = { 0, zoneHeight, width, zoneHeight * 2 };        
    mAreas[2] = { 0, zoneHeight * 2, width, height };


    int halfHeight = height / 2;

    // Définition des zones
    mAreas[0] = { 0, 0, width, halfHeight };              
    mAreas[1] = { 0, halfHeight, width, height };         
    mAreas[2] = { 0, halfHeight / 2, width, halfHeight + halfHeight / 2 };

    // Dessiner le terrain
    DrawField();

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
    // Dessiner les zones 
    for (int i = 0; i < LANE_COUNT; i++)
    {
        const AABB& aabb = mAreas[i];
        sf::Color color;
        if (i == 0) color = sf::Color::Red;     // Rouge pour la première zone
        else if (i == 1) color = sf::Color::Blue; // Bleu pour la deuxième zone
        else if (i == 2) color = sf::Color::Yellow; // Jaune pour la troisième zone

        Debug::DrawRectangle(aabb.xMin, aabb.yMin, aabb.xMax - aabb.xMin, aabb.yMax - aabb.yMin, color);
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
        player->SetPosition(width * 0.1f, (i + 1) * height / 6); // Positions espac�es verticalement
		player->SetTag(PLAYER_TEAM1);
        mTeam1.push_back(player);
    }

    for (int i = 0; i < 5; ++i)
    {
        Player* player = CreateEntity<Player>(playerRadius, sf::Color::Red);
        player->SetPosition(width * 0.9f, (i + 1) * height / 6);
        player->SetTag(PLAYER_TEAM2);
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
#include "RugbyScene.h"
#include "Player.h"
#include "Ball.h"
#include "Debug.h"
#include "GameManager.h"

void RugbyScene::OnInitialize()
{
    // Taille de la fen�tre
    int width = GetWindowWidth();
    int height = GetWindowHeight();

    // Dessiner le terrain
    DrawField();

    // Initialiser les �quipes
    InitializeTeams();

    // Cr�er la balle
    mBall = CreateEntity<Ball>(10.0f, sf::Color::Magenta);
    mBall->SetPosition(width / 2, height / 2);
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
                player->SetPosition(event.mouseButton.x, event.mouseButton.y);
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
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        // Rel�cher le joueur d�plac�
        if (mSelectedPlayer)
        {
            mSelectedPlayer = nullptr;
        }
    }
}


void RugbyScene::OnUpdate()
{
    // Mettre � jour les joueurs de l'�quipe 1
    for (Player* player : mTeam1)
    {
        player->Update();
    }

    // Mettre � jour les joueurs de l'�quipe 2
    for (Player* player : mTeam2)
    {
        player->Update();
    }

    // Mettre � jour la balle
    if (mBall)
    {
        mBall->Update();
    }

    // D�placer le joueur s�lectionn� avec la souris
    if (mSelectedPlayer)
    {
        // Obtenir la position globale de la souris
        sf::Vector2i mousePosition = sf::Mouse::getPosition(); // Utilisation brute des coordonn�es globales
        Debug::DrawCircle(mousePosition.x, mousePosition.y, 10, sf::Color::Red); // Indicateur visuel pour la position
        mSelectedPlayer->SetPosition(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
    }
}

void RugbyScene::InitializeTeams()
{
    int width = GetWindowWidth();
    int height = GetWindowHeight();
    float playerRadius = 20.0f;

    // Cr�er les joueurs de l'�quipe 1
    for (int i = 0; i < 5; ++i)
    {
        Player* player = CreateEntity<Player>(playerRadius, sf::Color::Green);
        player->SetPosition(width * 0.1f, (i + 1) * height / 6); // Positions espac�es verticalement
		player->SetTag(PLAYER_TEAM1);
        mTeam1.push_back(player);
    }

    // Cr�er les joueurs de l'�quipe 2
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
    Debug::DrawLine(width * 0.1f, 0, width * 0.1f, height, sf::Color::White); // Ligne c�t� gauche
    Debug::DrawLine(width * 0.9f, 0, width * 0.9f, height, sf::Color::White); // Ligne c�t� droit
}

#include "RugbyScene.h"
#include "Player.h"
#include "Ball.h"
#include "Debug.h"

void RugbyScene::OnInitialize()
{
    // Taille de la fenêtre
    int width = GetWindowWidth();
    int height = GetWindowHeight();

    // Dessiner le terrain
    DrawField();

    // Initialiser les équipes
    InitializeTeams();

    // Créer la balle
    mBall = CreateEntity<Ball>(10.0f, sf::Color::Magenta);
    mBall->SetPosition(width / 2, height / 2);
}

void RugbyScene::OnEvent(const sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        // Test pour sélectionner un joueur ou passer la balle
        for (Player* player : mTeam1)
        {
            if (player->IsInside(event.mouseButton.x, event.mouseButton.y))
            {
                player->SetHasBall(true);
                mBall->SetPosition(player->GetPosition().x, player->GetPosition().y);
                mBall->SetTag(BALL);
            }
        }
    }
}

void RugbyScene::OnUpdate()
{
    // Mettre à jour les joueurs de l'équipe 1
    for (Player* player : mTeam1)
    {
        player->Update();
    }

    // Mettre à jour les joueurs de l'équipe 2
    for (Player* player : mTeam2)
    {
        player->Update();
    }

    // Mettre à jour la balle
    if (mBall)
    {
        mBall->Update();
    }
}

void RugbyScene::InitializeTeams()
{
    int width = GetWindowWidth();
    int height = GetWindowHeight();
    float playerRadius = 20.0f;

    // Créer les joueurs de l'équipe 1
    for (int i = 0; i < 5; ++i)
    {
        Player* player = CreateEntity<Player>(playerRadius, sf::Color::Green);
        player->SetPosition(width * 0.1f, (i + 1) * height / 6); // Positions espacées verticalement
		player->SetTag(PLAYER_TEAM1);
        mTeam1.push_back(player);
    }

    // Créer les joueurs de l'équipe 2
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
    // Réinitialiser les positions des joueurs et de la balle après un essai
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

    // Réinitialiser les positions après la mise à jour du score
    ResetPositions();
}

void RugbyScene::DrawField()
{
    int width = GetWindowWidth();
    int height = GetWindowHeight();

    // Fond bleu
    Debug::DrawRectangle(0, 0, width, height, sf::Color::Blue);

    // Lignes blanches délimitant les zones
    Debug::DrawLine(width * 0.1f, 0, width * 0.1f, height, sf::Color::White); // Ligne côté gauche
    Debug::DrawLine(width * 0.9f, 0, width * 0.9f, height, sf::Color::White); // Ligne côté droit
}

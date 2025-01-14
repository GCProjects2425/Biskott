#include "PVZScene.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include "Player.h"
#include "Ball.h"
#include "Debug.h"

#define PLAN_COUNT 5
#define PLAYER_COUNT 5

void PVZScene::OnInitialize()
{
	int width = GetWindowWidth();
	int height = GetWindowHeight();

	float plantRadius = height * 0.050f;

	float plantStartY = height / (PLAN_COUNT * 2.f);
	float plantGapY = height / (float)PLAN_COUNT;

	float plantStartX = width * 0.05f;

	Player* mPlayers[PLAYER_COUNT*2];

	/*sf::Vector2f teamBluePos[PLAYER_COUNT] = {
		{height * 0.05f, width * 0.01f}, {50, height / 2.f - 30}, {50, height - 100.f}
	};

	sf::Vector2f teamRedPos[PLAYER_COUNT] = {
		{width - 50.f, 50}, {width - 60.f, height / 2.f - 30}, {width - 60.f, height - 100.f}
	};*/

	std::vector<sf::Vector2f> teamGreenPositions;
	std::vector<sf::Vector2f> teamRedPositions;

	float horizontalPadding = 50.f; // Distance par rapport aux zones d'essai
	float verticalSpacing = height / 6.f; // Espacement vertical entre les joueurs

	for (int i = 0; i < PLAYER_COUNT; ++i) {
		teamGreenPositions.emplace_back(horizontalPadding, verticalSpacing * (i + 1));
		teamRedPositions.emplace_back(width - horizontalPadding - (plantRadius*2), verticalSpacing * (i + 1));
	}

	for (int i = 0; i < (PLAYER_COUNT*2); i++)
	{
		sf::Vector2f teamPlayerPos = i < PLAYER_COUNT ? teamGreenPositions[i] : teamRedPositions[i-PLAYER_COUNT];
		mPlayers[i] = CreateEntity<Player>(plantRadius, sf::Color::White);
		mPlayers[i]->SetPosition(teamPlayerPos.x, teamPlayerPos.y, 0.f, 0.5f);
		mPlayers[i]->SetAreaIndex(i);

		if(i < PLAYER_COUNT)
		{
			mPlayers[i]->SetTag(PVZScene::Tag::PLAYER_RED);
		}
		else
		{
			mPlayers[i]->SetTag(PVZScene::Tag::PLAYER_BLUE);
		}

		int xMin = plantStartX + plantRadius * 3.f;
		int yMin = plantStartY - plantRadius;
		int xMax = width;
		int yMax = plantStartY + plantRadius;

		mAreas[i] = { xMin, yMin, xMax, yMax };

		plantStartY += plantGapY;
	}

	Ball* pBall = CreateEntity<Ball>(5.0f, sf::Color::Magenta);
	pBall->SetPosition(width / 2.f, height / 2.f);
}

void PVZScene::OnUpdate()
{
	for (int i = 0; i < PLAN_COUNT; i++)
	{
		const AABB& aabb = mAreas[i];

		//Debug::DrawRectangle(aabb.xMin, aabb.yMin, aabb.xMax - aabb.xMin, aabb.yMax - aabb.yMin, sf::Color::Red);
	}
}

int PVZScene::GetClickedArea(int x, int y) const
{
	for (int i = 0; i < PLAN_COUNT; i++)
	{
		const AABB* aabb = &mAreas[i];

		if (x >= aabb->xMin && x <= aabb->xMax && y >= aabb->yMin && y <= aabb->yMax)
			return i;
	}

	return -1;
}

void PVZScene::OnEvent(const sf::Event& event)
{
	if (event.type != sf::Event::EventType::MouseButtonPressed)
		return;

	int index = GetClickedArea(event.mouseButton.x, event.mouseButton.y);

	if(index == -1)
		return;

	const AABB* clickedArea = &mAreas[index];

	int y = clickedArea->yMin + (clickedArea->yMax - clickedArea->yMin) / 2;

	/*Zombie* pZombie = CreateEntity<Zombie>(25, sf::Color::Red);
	pZombie->SetPosition(event.mouseButton.x, y, 0.5f, 0.5f);
	pZombie->SetLane(index);*/

	mLaneZombieCount[index]++;
}

bool PVZScene::IsZombieInArea(int index) const
{
	_ASSERT(index >= 0 && index < PLAYER_COUNT);

	return mLaneZombieCount[index] > 0;
}

void PVZScene::OnDestroyZombie(int lane)
{
	_ASSERT(lane >= 0 && lane < PLAYER_COUNT);
	if(mLaneZombieCount[lane] <= 0)
		return;

	mLaneZombieCount[lane]--;
}
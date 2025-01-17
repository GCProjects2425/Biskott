#include "Ball.h"

void Ball::SetOwner(Player* owner)
{
    if (!mOwner)
        mOwner = owner;
    else
        mOwner->SetHasBall(false);

	mLastOwner = mOwner;

	owner->SetHasBall(true);
	mOwner = owner;
    SetIsAlreadySwitched(true);
}

int Ball::IsScoringATry() const
{
	int teamIdx = mOwner->IsTag(RugbyScene::PLAYER_TEAM1) ? RugbyScene::PLAYER_TEAM1 : RugbyScene::PLAYER_TEAM2;
	if (teamIdx == RugbyScene::PLAYER_TEAM1 && GetPosition().x >= GetScene<RugbyScene>()->GetWindowWidth() * 0.9f)
		return RugbyScene::PLAYER_TEAM1;
	if (teamIdx == RugbyScene::PLAYER_TEAM2 && GetPosition().x <= GetScene<RugbyScene>()->GetWindowWidth() * 0.1f)
		return RugbyScene::PLAYER_TEAM2;
	if (GetPosition().x >= GetScene<RugbyScene>()->GetWindowWidth() * 0.9f || GetPosition().x <= GetScene<RugbyScene>()->GetWindowWidth() * 0.1f)
		return 2;
	return -1;
}

void Ball::OnUpdate()
{
    if(mOwner && !IsMoving())
    {
        SetPosition(mOwner->GetPosition().x, mOwner->GetPosition().y);
    }
    else if (mOwner && IsMoving())
    {
        GoToPosition(mOwner->GetPosition().x, mOwner->GetPosition().y, mLastOwner->GetStats().passPower * 1000.f);
		if (mOwner->GetPosition().x == GetPosition().x && mOwner->GetPosition().y == GetPosition().y)
		{
			SetIsMoving(false);
		}
    }
    SetIsAlreadySwitched(false);
    // Logique de déplacement ou de gestion de la balle
}
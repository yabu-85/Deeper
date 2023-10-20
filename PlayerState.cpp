#include "PlayerState.h"
#include "Player.h"

PlayerAvo::PlayerAvo(GameObject* owner)
	: StateBase(owner)
{
	owner_ = owner;
}

void PlayerAvo::Update()
{
	Player* pPlayer = static_cast<Player*>(owner_);
	pPlayer->SetRotateY(pPlayer->GetRotate().x + 10.0f);
}

void PlayerAvo::OnEnter()
{
}

void PlayerAvo::OnExit()
{
}
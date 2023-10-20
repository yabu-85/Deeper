#include "PlayerState.h"
#include "Player.h"
#include "StateManager.h"
#include "Engine/Input.h"

namespace {
	const int avoTime = 5;
	const float avoSpeed = 3.0f;
}

void PlayerWait::Update()
{
	Player* pPlayer = static_cast<Player*>(owner_->GetGameObject());
	pPlayer->CalcNoMove();
	pPlayer->Move();

	//キー入力でステート切り替え
	if (pPlayer->IsMoveKeyPushed())
	{
		owner_->ChangeState("Walk");
	}
	if (Input::IsKeyDown(DIK_SPACE)) {
		owner_->ChangeState("Avo");
	}
}

//------------------------------------------------------------

void PlayerWalk::Update()
{
	Player* pPlayer = static_cast<Player*>(owner_->GetGameObject());
	pPlayer->CalcMove();
	pPlayer->Move();

	if (!pPlayer->IsMoveKeyPushed())
	{
		owner_->ChangeState("Wait");
	}
	if (Input::IsKeyDown(DIK_SPACE)) {
		owner_->ChangeState("Avo");
	}

}

//------------------------------------------------------------

void PlayerAvo::Update()
{
	Player* pPlayer = static_cast<Player*>(owner_->GetGameObject());
	pPlayer->Move(avoSpeed);

	avoTime_--;
	if(avoTime_ <= 0) owner_->ChangeState("Wait");

}

void PlayerAvo::OnEnter()
{
	Player* pPlayer = static_cast<Player*>(owner_->GetGameObject());
	if (pPlayer->IsMoveKeyPushed()) {
		pPlayer->CalcMove();
	}
	else {
		//移動キー押してない場合は向いている方向にする

	}

	avoTime_ = avoTime;
}
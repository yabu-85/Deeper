#include "PlayerState.h"
#include "Player.h"
#include "StateManager.h"
#include "Engine/Input.h"

namespace {
	const int defAvoTime = 30;
	const float defAvoSpeed = 2.0f;
}

void PlayerWait::Update()
{
	//ƒL[“ü—Í‚ÅƒXƒe[ƒgØ‚è‘Ö‚¦
	if (pPlayer_->IsMoveKeyPushed()) {
		owner_->ChangeState("Walk");
		return;
	}
	if (Input::IsKeyDown(DIK_SPACE)) {
		owner_->ChangeState("Avo");
		return;
	}

	pPlayer_->CalcNoMove();
	pPlayer_->Move();
}

void PlayerWait::Initialize()
{
	pPlayer_ = static_cast<Player*>(owner_->GetGameObject());
}

//------------------------------------------------------------

void PlayerWalk::Update()
{
	pPlayer_->CalcMove();
	pPlayer_->Move();

	if (!pPlayer_->IsMoveKeyPushed()) {
		owner_->ChangeState("Wait");
		return;
	}
	if (Input::IsKeyDown(DIK_SPACE)) {
		owner_->ChangeState("Avo");
		return;
	}
}

void PlayerWalk::Initialize()
{
	pPlayer_ = static_cast<Player*>(owner_->GetGameObject());
}

//------------------------------------------------------------

void PlayerAvo::Update()
{
	avoTime_--;
	if (avoTime_ <= 0) {
		owner_->ChangeState("Wait");
		return;
	}

	const float endValue = 0.1f; //‚PD‚O`‚±‚Ì’lƒK‚Æ‚ê‚é‚æ‚¤‚É‚·‚é
	float t = (float)avoTime_ / (float)defAvoTime;
	float value = endValue + ((1.0f - endValue) * t);
	pPlayer_->Move(value * defAvoSpeed);
}

void PlayerAvo::OnEnter()
{
	pPlayer_->InitAvo();
	
	avoTime_ = defAvoTime;
}

void PlayerAvo::OnExit()
{
	pPlayer_->ResetMovement();
}

void PlayerAvo::Initialize()
{
	pPlayer_ = static_cast<Player*>(owner_->GetGameObject());
}

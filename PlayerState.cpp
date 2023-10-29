#include "PlayerState.h"
#include "Player.h"
#include "StateManager.h"
#include "PlayerCommand.h"
#include "DamageCtrl.h"
#include "GameManager.h"

namespace {
	const int defAvoTime = 30;
	const float defAvoSpeed = 2.0f;
}

//--------------------------------------------------------------------------------

PlayerWait::PlayerWait(StateManager* owner)
{
	owner_ = owner;
	pPlayer_ = static_cast<Player*>(owner_->GetGameObject());
}

void PlayerWait::Update()
{
	//ƒL[“ü—Í‚ÅƒXƒe[ƒgØ‚è‘Ö‚¦
	if (pPlayer_->GetCommand()->CmdWalk()) {
		owner_->ChangeState("Walk");
		return;
	}
	if (pPlayer_->GetCommand()->CmdAvo()) {
		owner_->ChangeState("Avo");
		return;
	}
	if (pPlayer_->GetCommand()->CmdAtk()) {
		owner_->ChangeState("Atk");
		return;
	}

	pPlayer_->CalcNoMove();
	pPlayer_->Move();
}

//--------------------------------------------------------------------------------

PlayerWalk::PlayerWalk(StateManager* owner)
{
	owner_ = owner;
	pPlayer_ = static_cast<Player*>(owner_->GetGameObject());
}

void PlayerWalk::Update()
{
	pPlayer_->CalcMove();
	pPlayer_->Move();
	pPlayer_->Rotate();

	if (!pPlayer_->GetCommand()->CmdWalk()) {
		owner_->ChangeState("Wait");
		return;
	}
	if (pPlayer_->GetCommand()->CmdAvo()) {
		owner_->ChangeState("Avo");
		return;
	}
	if (pPlayer_->GetCommand()->CmdAtk()) {
		owner_->ChangeState("Atk");
		return;
	}
}

//--------------------------------------------------------------------------------

PlayerAvo::PlayerAvo(StateManager* owner)
	:avoTime_(0)
{
	owner_ = owner;
	pPlayer_ = static_cast<Player*>(owner_->GetGameObject());
}

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

//--------------------------------------------------------------------------------

PlayerAtk::PlayerAtk(StateManager* owner)
	:atkTime_(0)
{
	owner_ = owner;
	pPlayer_ = static_cast<Player*>(owner_->GetGameObject());
	
	GameManager* pGameManager = (GameManager*)pPlayer_->FindObject("GameManager");
	pDamageCtrl_ = pGameManager->GetDamageCtrl();
}

void PlayerAtk::Update()
{
	atkTime_--;
	if (atkTime_ <= 0) {
		owner_->ChangeState("Wait");
		return;
	}

	if(pPlayer_->GetCommand()->CmdAtk())
	pDamageCtrl_->ApplyDamage(DamageCtrl::ALL, 4);

	pPlayer_->SetScale(XMFLOAT3( 1.0f - (float)atkTime_ / (float)60, 1.0f, 1.0f - (float)atkTime_ / (float)60) );
	pPlayer_->CalcNoMove();
}

void PlayerAtk::OnEnter()
{
	pDamageCtrl_->ApplyDamage(DamageCtrl::ALL, 4);
	atkTime_ = 60;
}

void PlayerAtk::OnExit()
{
}

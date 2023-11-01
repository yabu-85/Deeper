#include "PlayerState.h"
#include "Player.h"
#include "StateManager.h"
#include "PlayerCommand.h"
#include "GameManager.h"
#include "WeaponBase.h"

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
	//キー入力でステート切り替え
	if (pPlayer_->GetCommand()->CmdWalk()) {
		owner_->ChangeState("Walk");
		return;
	}
	if (pPlayer_->GetCommand()->CmdAvo()) {
		owner_->ChangeState("Avo");
		return;
	}
	if (pPlayer_->GetMainWeapon() && pPlayer_->GetCommand()->CmdAtk()) {
		owner_->ChangeState("Atk");
		return;
	}
	if (pPlayer_->GetSubWeapon() && pPlayer_->GetCommand()->CmdSubAtk()) {
		owner_->ChangeState("SubAtk");
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
	if (pPlayer_->GetMainWeapon() && pPlayer_->GetCommand()->CmdAtk()) {
		owner_->ChangeState("Atk");
		return;
	}
	if (pPlayer_->GetSubWeapon() && pPlayer_->GetCommand()->CmdSubAtk()) {
		owner_->ChangeState("SubAtk");
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

	const float endValue = 0.1f; //１．０～この値ガとれるようにする
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
	:nextCmd_(0)
{
	owner_ = owner;
	pPlayer_ = static_cast<Player*>(owner_->GetGameObject());
}

void PlayerAtk::Update()
{
	pPlayer_->CalcNoMove();
	pPlayer_->GetMainWeapon()->UpdateState();

	if (pPlayer_->GetCommand()->CmdAvo()) nextCmd_ = 1;
	if (pPlayer_->GetCommand()->CmdSubAtk()) nextCmd_ = 2;
	
	//コンボ終了NextCmdのステートへ
	if (pPlayer_->GetMainWeapon()->IsAtkEnd()) {
		if (nextCmd_ == 1) {
			owner_->ChangeState("Avo");
			return;
		}
		if (nextCmd_ == 2) {
			owner_->ChangeState("SubAtk");
			return;
		}
		owner_->ChangeState("Wait");
		return;
	}
}

void PlayerAtk::OnEnter()
{
	nextCmd_ = 0;
	pPlayer_->GetMainWeapon()->SetAtkEnd(false);

}

void PlayerAtk::OnExit()
{
	pPlayer_->GetMainWeapon()->ResetState();
}
 
//--------------------------------------------------------------------------------

PlayerSubAtk::PlayerSubAtk(StateManager* owner)
	:nextCmd_(0)
{
	owner_ = owner;
	pPlayer_ = static_cast<Player*>(owner_->GetGameObject());
}

void PlayerSubAtk::Update()
{
	pPlayer_->CalcMove();
	pPlayer_->Move();
	pPlayer_->GetSubWeapon()->UpdateState();

	//クールタイム終わり
	if (pPlayer_->GetSubWeapon()->IsAtkEnd()) {
		owner_->ChangeState("Walk");
		return;
	}
}

void PlayerSubAtk::OnEnter()
{
	nextCmd_ = 0;
	pPlayer_->GetSubWeapon()->SetAtkEnd(false);
}

void PlayerSubAtk::OnExit()
{
	pPlayer_->GetSubWeapon()->ResetState();
}

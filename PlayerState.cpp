#include "PlayerState.h"
#include "Player.h"
#include "StateManager.h"
#include "PlayerCommand.h"
#include "GameManager.h"
#include "WeaponBase.h"
#include "WeaponObjectManager.h"
#include "Aim.h"
#include "PlayerWeapon.h"

namespace {
	const int defAvoTime = 30;
	const float defAvoSpeed = 2.0f;
}

//--------------------------------------------------------------------------------

PlayerWait::PlayerWait(StateManager* owner) : StateBase(owner)
{
	pPlayer_ = static_cast<Player*>(owner_->GetGameObject());
}

void PlayerWait::Update()
{
	pPlayer_->GetPlayerWeapon()->WeaponChangeIndex();
	pPlayer_->CalcNoMove();
	pPlayer_->Move();
	if (pPlayer_->GetAim()->IsTarget()) pPlayer_->AimTargetRotate();

	//キー入力でステート切り替え
	if (pPlayer_->GetCommand()->CmdWalk()) {
		owner_->ChangeState("Walk");
		return;
	}
	if (pPlayer_->GetCommand()->CmdAvo()) {
		owner_->ChangeState("Avo");
		return;
	}
	if (pPlayer_->GetPlayerWeapon()->GetMainWeapon() && pPlayer_->GetCommand()->CmdAtk()) {
		owner_->ChangeState("Atk");
		return;
	}
	if (pPlayer_->GetPlayerWeapon()->GetSubWeapon() && pPlayer_->GetCommand()->CmdSubAtk()) {
		owner_->ChangeState("SubAtk");
		return;
	}
	if (pPlayer_->GetCommand()->CmdPushAction()) {
		if (GameManager::GetWeaponObjectManager()->IsInPlayerRange()) {
			owner_->ChangeState("Change");
			return;
		}
	}
}

//--------------------------------------------------------------------------------

PlayerWalk::PlayerWalk(StateManager* owner) : StateBase(owner)
{
	pPlayer_ = static_cast<Player*>(owner_->GetGameObject());
}

void PlayerWalk::Update()
{
	if (!pPlayer_->GetCommand()->CmdWalk()) {
		owner_->ChangeState("Wait");
		return;
	}
	if (pPlayer_->GetCommand()->CmdAvo()) {
		owner_->ChangeState("Avo");
		return;
	}
	if (pPlayer_->GetPlayerWeapon()->GetMainWeapon() && pPlayer_->GetCommand()->CmdAtk()) {
		owner_->ChangeState("Atk");
		return;
	}
	if (pPlayer_->GetPlayerWeapon()->GetSubWeapon() && pPlayer_->GetCommand()->CmdSubAtk()) {
		owner_->ChangeState("SubAtk");
		return;
	}
	if (pPlayer_->GetCommand()->CmdPushAction()) {
		if (GameManager::GetWeaponObjectManager()->IsInPlayerRange()) {
			owner_->ChangeState("Change");
			return;
		}
	}
	
	pPlayer_->GetPlayerWeapon()->WeaponChangeIndex();
	pPlayer_->CalcMove();
	pPlayer_->Move(); 

	if (pPlayer_->GetAim()->IsTarget()) pPlayer_->AimTargetRotate();
	else pPlayer_->Rotate();

}

//--------------------------------------------------------------------------------

namespace {
	static const float CHANGE_TIME = 60;
}

PlayerWeaponChange::PlayerWeaponChange(StateManager* owner) : StateBase(owner), time_(0)
{
	pPlayer_ = static_cast<Player*>(owner_->GetGameObject());
}

void PlayerWeaponChange::Update()
{
	pPlayer_->CalcNoMove();
	pPlayer_->Move();

	if (pPlayer_->GetCommand()->CmdPushAction()) {
		time_++;

		//切り替え時間までボタン押し続けた
		if (time_ > CHANGE_TIME) {
			WeaponBase* weapon = GameManager::GetWeaponObjectManager()->GetNearestWeapon();
			if (weapon) {
				pPlayer_->GetPlayerWeapon()->SetWeapon(weapon);
			}

			owner_->ChangeState("Wait");
			return;
		}

		//押してる
		return;
	}

	//押してないからWaitへ
	owner_->ChangeState("Wait");
}

void PlayerWeaponChange::OnEnter()
{
	time_ = 0;
}

//--------------------------------------------------------------------------------

PlayerAvo::PlayerAvo(StateManager* owner) : StateBase(owner), avoTime_(0), nextCmd_(0)
{
	pPlayer_ = static_cast<Player*>(owner_->GetGameObject());
}

void PlayerAvo::Update()
{
	const float endValue = 0.1f; //１．０～この値ガとれるようにする
	float t = (float)avoTime_ / (float)defAvoTime;
	float value = endValue + ((1.0f - endValue) * t);
	pPlayer_->Move(value * defAvoSpeed);

	if (pPlayer_->GetCommand()->CmdAvo()) nextCmd_ = 1;
	if (pPlayer_->GetCommand()->CmdAtk()) nextCmd_ = 2;
	if (pPlayer_->GetCommand()->CmdSubAtk()) nextCmd_ = 3;

	avoTime_--;
	if (avoTime_ <= 0) {
		if (nextCmd_ == 1) {
			owner_->ChangeState("Avo");
			return;
		}
		if (pPlayer_->GetPlayerWeapon()->GetMainWeapon() && nextCmd_ == 2) {
			owner_->ChangeState("Atk");
			return;
		}
		if (pPlayer_->GetPlayerWeapon()->GetSubWeapon() && nextCmd_ == 3) {
			owner_->ChangeState("SubAtk");
			return;
		}
		if (pPlayer_->GetCommand()->CmdWalk()) {
			owner_->ChangeState("Walk");
			return;
		}
		owner_->ChangeState("Wait");
		return;
	}
}

void PlayerAvo::OnEnter()
{
	pPlayer_->InitAvo();
	nextCmd_ = 0;
	avoTime_ = defAvoTime;
}

void PlayerAvo::OnExit()
{
	pPlayer_->ResetKeyMovement();
}

//--------------------------------------------------------------------------------

PlayerAtk::PlayerAtk(StateManager* owner) : StateBase(owner), nextCmd_(0), time_(0)
{
	pPlayer_ = static_cast<Player*>(owner_->GetGameObject());
}

void PlayerAtk::Update()
{
	pPlayer_->GetPlayerWeapon()->GetMainWeapon()->UpdateState();

	if (pPlayer_->GetCommand()->CmdAtk()) nextCmd_ = 0;
	if (pPlayer_->GetCommand()->CmdAvo()) nextCmd_ = 1;
	if (pPlayer_->GetCommand()->CmdSubAtk()) nextCmd_ = 2;
	
	if (time_ < 10 && nextCmd_ == 1) {
		owner_->ChangeState("Avo");
		return;
	}
	time_++;

	//コンボ終了NextCmdのステートへ
	if (pPlayer_->GetPlayerWeapon()->GetMainWeapon()->IsAtkEnd()) {
		if (nextCmd_ == 1) {
			owner_->ChangeState("Avo");
			return;
		}
		if (pPlayer_->GetPlayerWeapon()->GetSubWeapon() && nextCmd_ == 2) {
			owner_->ChangeState("SubAtk");
			return;
		}
		if (pPlayer_->GetCommand()->CmdWalk()) {
			owner_->ChangeState("Walk");
			return;
		}
		owner_->ChangeState("Wait");
		return;
	}
}

void PlayerAtk::OnEnter()
{
	time_ = 0;
	nextCmd_ = 0;
	pPlayer_->GetPlayerWeapon()->GetMainWeapon()->SetAtkEnd(false);

}

void PlayerAtk::OnExit()
{
	pPlayer_->GetPlayerWeapon()->GetMainWeapon()->ResetState();
}
 
//--------------------------------------------------------------------------------

PlayerSubAtk::PlayerSubAtk(StateManager* owner) : StateBase(owner), nextCmd_(0)
{
	pPlayer_ = static_cast<Player*>(owner_->GetGameObject());
}

void PlayerSubAtk::Update()
{
	pPlayer_->GetPlayerWeapon()->GetSubWeapon()->UpdateState();

	if (pPlayer_->GetCommand()->CmdSubAtk()) nextCmd_ = 0;
	if (pPlayer_->GetCommand()->CmdAvo()) nextCmd_ = 1;
	if (pPlayer_->GetCommand()->CmdAtk()) nextCmd_ = 2;

	//コンボ終了NextCmdのステートへ
	if (pPlayer_->GetPlayerWeapon()->GetSubWeapon()->IsAtkEnd()) {
		if (nextCmd_ == 1) {
			owner_->ChangeState("Avo");
			return;
		}
		if (pPlayer_->GetPlayerWeapon()->GetMainWeapon() && nextCmd_ == 2) {
			owner_->ChangeState("Atk");
			return;
		}
		if (pPlayer_->GetCommand()->CmdWalk()) {
			owner_->ChangeState("Walk");
			return;
		}
		owner_->ChangeState("Wait");
		return;
	}
}

void PlayerSubAtk::OnEnter()
{
	nextCmd_ = 0;
	pPlayer_->GetPlayerWeapon()->GetSubWeapon()->SetAtkEnd(false);
}

void PlayerSubAtk::OnExit()
{
	if (pPlayer_->GetPlayerWeapon()->GetSubWeapon()->IsBlockend()) {
		pPlayer_->GetPlayerWeapon()->SubWeaponRemove();
		return;
	}
	pPlayer_->GetPlayerWeapon()->GetSubWeapon()->ResetState();

}

//--------------------------------------------------------------------------------

PlayerDead::PlayerDead(StateManager* owner) : StateBase(owner)
{
	pPlayer_ = static_cast<Player*>(owner_->GetGameObject());
}

void PlayerDead::Update()
{
	pPlayer_->CalcNoMove();
}

void PlayerDead::OnEnter()
{
	pPlayer_->GetAim()->SetAimMove(false);
}

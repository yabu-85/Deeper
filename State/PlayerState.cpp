#include "PlayerState.h"
#include "../Player/Player.h"
#include "StateManager.h"
#include "../Player/PlayerCommand.h"
#include "../GameManager.h"
#include "../Weapon/WeaponBase.h"
#include "../Weapon/WeaponObjectManager.h"
#include "../Player/Aim.h"
#include "../Player/PlayerWeapon.h"
#include "../Engine/Model.h"
#include "../AnimationController.h"

namespace {
	const int defAvoTime = 30;
	const float defAvoSpeed = 2.0f;
}

//--------------------------------------------------------------------------------

PlayerWait::PlayerWait(StateManager* owner) : StateBase(owner)
{
}

void PlayerWait::Update()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->GetPlayerWeapon()->WeaponChangeIndex();
	p->CalcNoMove();
	p->Move();
	if (p->GetAim()->IsTarget()) p->AimTargetRotate();

	//キー入力でステート切り替え
	if (p->GetCommand()->CmdWalk()) {
		owner_->ChangeState("Walk");
		return;
	}
	if (p->GetCommand()->CmdAvo()) {
		owner_->ChangeState("Avo");
		return;
	}
	if (p->GetPlayerWeapon()->GetMainWeapon() && p->GetCommand()->CmdAtk()) {
		owner_->ChangeState("Atk");
		return;
	}
	if (p->GetPlayerWeapon()->GetSubWeapon() && p->GetCommand()->CmdSubAtk()) {
		owner_->ChangeState("SubAtk");
		return;
	}
	if (p->GetCommand()->CmdPushAction()) {
		if (GameManager::GetWeaponObjectManager()->IsInPlayerRange()) {
			owner_->ChangeState("Change");
			return;
		}
	}
}

void PlayerWait::OnEnter()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	float weight = 1.0f - Model::GetBlendFactor(p->GetModelHandle());
	p->GetAnimationController()->SetNextAnime(0, Model::GetAnimFrame(p->GetModelHandle()), weight, 0.1f);

}

//--------------------------------------------------------------------------------

PlayerWalk::PlayerWalk(StateManager* owner) : StateBase(owner)
{
}

void PlayerWalk::Update()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	if (!p->GetCommand()->CmdWalk()) {
		owner_->ChangeState("Wait");
		return;
	}
	if (p->GetCommand()->CmdAvo()) {
		owner_->ChangeState("Avo");
		return;
	}
	if (p->GetPlayerWeapon()->GetMainWeapon() && p->GetCommand()->CmdAtk()) {
		owner_->ChangeState("Atk");
		return;
	}
	if (p->GetPlayerWeapon()->GetSubWeapon() && p->GetCommand()->CmdSubAtk()) {
		owner_->ChangeState("SubAtk");
		return;
	}
	if (p->GetCommand()->CmdPushAction()) {
		if (GameManager::GetWeaponObjectManager()->IsInPlayerRange()) {
			owner_->ChangeState("Change");
			return;
		}
	}
	
	p->GetPlayerWeapon()->WeaponChangeIndex();
	p->CalcMove();
	p->Move(); 
	p->Rotate();

}

void PlayerWalk::OnEnter()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->GetAnimationController()->SetNextAnime(1, 0, 1.0f, 0.05f);
	
}

//--------------------------------------------------------------------------------

namespace {
	static const float CHANGE_TIME = 60;
}

PlayerWeaponChange::PlayerWeaponChange(StateManager* owner) : StateBase(owner), time_(0)
{
}

void PlayerWeaponChange::Update()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->CalcNoMove();
	p->Move();

	if (p->GetCommand()->CmdPushAction()) {
		time_++;

		//切り替え時間までボタン押し続けた
		if (time_ >= CHANGE_TIME) {
			WeaponBase* weapon = GameManager::GetWeaponObjectManager()->PlayerWeaponSet();
			if (weapon) {
				p->GetPlayerWeapon()->SetWeapon(weapon);
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
}

void PlayerAvo::Update()
{
	const float endValue = 0.1f; //１．０～この値ガとれるようにする
	float t = (float)avoTime_ / (float)defAvoTime;
	float value = endValue + ((1.0f - endValue) * t);

	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->Move(value * defAvoSpeed);

	if (p->GetCommand()->CmdAvo()) nextCmd_ = 1;
	if (p->GetCommand()->CmdAtk()) nextCmd_ = 2;
	if (p->GetCommand()->CmdSubAtk()) nextCmd_ = 3;

	avoTime_--;
	if (avoTime_ <= 0) {
		if (nextCmd_ == 1) {
			owner_->ChangeState("Avo");
			return;
		}
		if (p->GetPlayerWeapon()->GetMainWeapon() && nextCmd_ == 2) {
			owner_->ChangeState("Atk");
			return;
		}
		if (p->GetPlayerWeapon()->GetSubWeapon() && nextCmd_ == 3) {
			owner_->ChangeState("SubAtk");
			return;
		}
		if (p->GetCommand()->CmdWalk()) {
			owner_->ChangeState("Walk");
			return;
		}
		owner_->ChangeState("Wait");
		return;
	}
}

void PlayerAvo::OnEnter()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->InitAvo();
	nextCmd_ = 0;
	avoTime_ = defAvoTime;

}

void PlayerAvo::OnExit()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->ResetKeyMovement();

}

//--------------------------------------------------------------------------------

PlayerAtk::PlayerAtk(StateManager* owner) : StateBase(owner), nextCmd_(0), time_(0)
{
}

void PlayerAtk::Update()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->GetPlayerWeapon()->GetMainWeapon()->UpdateState();

	if (p->GetCommand()->CmdAtk()) nextCmd_ = 0;
	if (p->GetCommand()->CmdAvo()) nextCmd_ = 1;
	if (p->GetCommand()->CmdSubAtk()) nextCmd_ = 2;
	
	if (time_ < 10 && nextCmd_ == 1) {
		owner_->ChangeState("Avo");
		return;
	}
	time_++;

	//コンボ終了NextCmdのステートへ
	if (p->GetPlayerWeapon()->GetMainWeapon()->IsAtkEnd()) {
		if (nextCmd_ == 1) {
			owner_->ChangeState("Avo");
			return;
		}
		if (p->GetPlayerWeapon()->GetSubWeapon() && nextCmd_ == 2) {
			owner_->ChangeState("SubAtk");
			return;
		}
		if (p->GetCommand()->CmdWalk()) {
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
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->GetPlayerWeapon()->GetMainWeapon()->ChangeAttackState();

}

void PlayerAtk::OnExit()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->GetPlayerWeapon()->GetMainWeapon()->ResetState();
}
 
//--------------------------------------------------------------------------------

PlayerSubAtk::PlayerSubAtk(StateManager* owner) : StateBase(owner), nextCmd_(0)
{
}

void PlayerSubAtk::Update()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->GetPlayerWeapon()->GetSubWeapon()->UpdateState();

	if (p->GetCommand()->CmdSubAtk()) nextCmd_ = 0;
	if (p->GetCommand()->CmdAvo()) nextCmd_ = 1;
	if (p->GetCommand()->CmdAtk()) nextCmd_ = 2;

	//コンボ終了NextCmdのステートへ
	if (p->GetPlayerWeapon()->GetSubWeapon()->IsAtkEnd()) {
		if (nextCmd_ == 1) {
			owner_->ChangeState("Avo");
			return;
		}
		if (p->GetPlayerWeapon()->GetMainWeapon() && nextCmd_ == 2) {
			owner_->ChangeState("Atk");
			return;
		}
		if (p->GetCommand()->CmdWalk()) {
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
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->GetPlayerWeapon()->GetSubWeapon()->ChangeAttackState();

}

void PlayerSubAtk::OnExit()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	if (p->GetPlayerWeapon()->GetSubWeapon()->IsBlockend()) {
		p->GetPlayerWeapon()->SubWeaponRemove();
		return;
	}
	p->GetPlayerWeapon()->GetSubWeapon()->ResetState();

}
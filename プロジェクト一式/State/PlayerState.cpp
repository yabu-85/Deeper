#include "PlayerState.h"
#include "StateManager.h"
#include "../AnimationController.h"
#include "../VFXManager.h"
#include "../GameManager/GameManager.h"
#include "../Engine/Model.h"
#include "../Engine/SceneManager.h"
#include "../Player/Player.h"
#include "../Player/PlayerCommand.h"
#include "../Player/Aim.h"
#include "../Player/PlayerWeapon.h"
#include "../Weapon/WeaponBase.h"
#include "../Weapon/WeaponObjectManager.h"

namespace {
	static const int AVO_TIME = 40;
	static const int INVINCIBLE_TIME = 15;
	static const int UP_COLLIDER_ACTIVE = 38;
	static const float AVO_SPEED = 2.0f;
	static const float CHANGE_TIME = 60;
	static const int HEAR_TIME = 50;

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
	p->GetAnimationController()->SetNextAnime(0, weight, 0.1f);

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
	float weight = 1.0f - Model::GetBlendFactor(p->GetModelHandle());
	p->GetAnimationController()->SetNextAnime(1, weight, 0.05f);

}

//--------------------------------------------------------------------------------

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
			WeaponBase* weapon = GameManager::GetWeaponObjectManager()->GetNearestWeapon();
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
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	float weight = 1.0f - Model::GetBlendFactor(p->GetModelHandle());
	p->GetAnimationController()->SetNextAnime(0, weight, 0.1f);

}

//--------------------------------------------------------------------------------

PlayerAvo::PlayerAvo(StateManager* owner) : StateBase(owner), avoTime_(0), nextCmd_(0)
{
}

void PlayerAvo::Update()
{
	const float endValue = 0.1f;
	float t = (float)avoTime_ / (float)AVO_TIME;
	t = 1.0f - t;
	float value = endValue + ((1.0f - endValue) * t);

	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->Move(value * AVO_SPEED);

	if (avoTime_ == INVINCIBLE_TIME) p->GetSphereCollider(0)->SetValid(true);
	if (avoTime_ == UP_COLLIDER_ACTIVE) p->GetSphereCollider(1)->SetValid(true);

	if (p->GetCommand()->CmdAvo()) nextCmd_ = 1;
	if (p->GetCommand()->CmdAtk()) nextCmd_ = 2;
	if (p->GetCommand()->CmdSubAtk()) nextCmd_ = 3;

	avoTime_++;
	if (avoTime_ >= AVO_TIME) {
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
	p->Avo();

	nextCmd_ = 0;
	avoTime_ = 0;

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
	p->GetPlayerWeapon()->GetMainWeapon()->Visible();
	if (p->GetPlayerWeapon()->GetSubWeapon()) p->GetPlayerWeapon()->GetSubWeapon()->Invisible();

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
	p->GetPlayerWeapon()->GetMainWeapon()->Invisible();
	if(p->GetPlayerWeapon()->GetSubWeapon()) p->GetPlayerWeapon()->GetSubWeapon()->Visible();

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

//--------------------------------------------------------------------------------

PlayerHear::PlayerHear(StateManager* owner) : StateBase(owner), time_(0), nextCmd_(0)
{
}

void PlayerHear::Update()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	float speed = 1.0f - ((float)time_ / (float)HEAR_TIME);
	p->BackMove(speed);

	time_++;
	if (time_ >= HEAR_TIME) {
		owner_->ChangeState("Wait");
	}

}

void PlayerHear::OnEnter()
{
	time_ = 0;
}

//--------------------------------------------------------------------------------

PlayerDead::PlayerDead(StateManager* owner) : StateBase(owner), time_(0)
{
}

void PlayerDead::Update()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	time_++;
	
	int s = p->GetAnimationController()->GetAnim(6).startFrame;
	int e = p->GetAnimationController()->GetAnim(6).endFrame;

	if (time_ >= (e - s)) {
		Model::SetAnimeStop(p->GetModelHandle(), true);
		GameManager::GetSceneManager()->ChangeScene(SCENE_ID_RESULT);
	}
}

void PlayerDead::OnEnter()
{
	time_ = 0;
}

//--------------------------------------------------------------------------------

PlayerAppear::PlayerAppear(StateManager* owner) : StateBase(owner), time_(0), apperPos_{0.0f, 0.0f, 0.0f}
{
}

void PlayerAppear::Update()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	time_++;
	
	XMFLOAT3 pos = p->GetPosition();
	owner_->GetGameObject()->SetPosition({ pos.x, pos.y - 0.5f, pos.z });

	XMFLOAT3 cPos = XMFLOAT3(apperPos_.x, 5.0f, apperPos_.z + 13.0f);
	p->GetAim()->SetCompulsion(cPos, apperPos_);

	if (time_ >= 60) {
		pos.y += 0.7f;
		VFXManager::CreatVfxExplode1(pos);
		owner_->ChangeState("Wait");
	}
}

void PlayerAppear::OnEnter()
{
	apperPos_ = owner_->GetGameObject()->GetPosition();
	time_ = 0;

}

//--------------------------------------------------------------------------------

PlayerDisAppear::PlayerDisAppear(StateManager* owner) : StateBase(owner), time_(0)
{
}

void PlayerDisAppear::Update()
{
	XMFLOAT3 pos = owner_->GetGameObject()->GetPosition();
	owner_->GetGameObject()->SetPosition({ pos.x, pos.y - 0.1f, pos.z });

}

void PlayerDisAppear::OnEnter()
{
}

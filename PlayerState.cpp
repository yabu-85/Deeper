#include "PlayerState.h"
#include "Player.h"
#include "StateManager.h"
#include "PlayerCommand.h"
#include "DamageCtrl.h"
#include "GameManager.h"
#include "TestBullet.h"

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
	if (pPlayer_->GetCommand()->CmdAtk()) {
		owner_->ChangeState("Atk");
		return;
	}
	if (pPlayer_->GetCommand()->CmdSubAtk()) {
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
	if (pPlayer_->GetCommand()->CmdAtk()) {
		owner_->ChangeState("Atk");
		return;
	}
	if (pPlayer_->GetCommand()->CmdSubAtk()) {
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
	:atkTime_(0)
{
	owner_ = owner;
	pPlayer_ = static_cast<Player*>(owner_->GetGameObject());
	GameManager* pGameManager = (GameManager*)pPlayer_->FindObject("GameManager");
	pDamageCtrl_ = pGameManager->GetDamageCtrl();
}

namespace {
	int StartTime = 60; 
	int nextCmdTime = 10;
	unsigned nextCmd = 0;
}

void PlayerAtk::Update()
{
	pPlayer_->CalcNoMove();
	atkTime_--;

	if (pPlayer_->GetCommand()->CmdAvo())
		nextCmd = 1;
	if (pPlayer_->GetCommand()->CmdAtk())
		nextCmd = 2;
	if (pPlayer_->GetCommand()->CmdSubAtk())
		nextCmd = 3;

	if (atkTime_ <= (int)nextCmd) {
		//Command
		if (nextCmd == 1) {
			owner_->ChangeState("Avo");
			return;
		}
		if (nextCmd == 2) {
			owner_->ChangeState("Atk");
			return;
		}
		if (nextCmd == 3) {
			owner_->ChangeState("SubAtk");
			return;
		}
		
		//クールタイム終わり
		if (atkTime_ <= 0) {
			owner_->ChangeState("Wait");
			return;
		}
	}

	pPlayer_->SetScale(XMFLOAT3(1.0f - (float)atkTime_ / (float)60, 1.0f, 1.0f - (float)atkTime_ / (float)60));
}

void PlayerAtk::OnEnter()
{
	pDamageCtrl_->ApplyDamage(DamageCtrl::ALL, 4);
	atkTime_ = 60;
	nextCmd = 0;
}

//--------------------------------------------------------------------------------

PlayerSubAtk::PlayerSubAtk(StateManager* owner)
	:atkTime_(0)
{
	owner_ = owner;
	pPlayer_ = static_cast<Player*>(owner_->GetGameObject());
	GameManager* pGameManager = (GameManager*)pPlayer_->FindObject("GameManager");
	pDamageCtrl_ = pGameManager->GetDamageCtrl();
}

void PlayerSubAtk::Update()
{
	atkTime_--;
	//クールタイム終わり
	if (atkTime_ <= 0) {
		owner_->ChangeState("Wait");
		return;
	}
}

void PlayerSubAtk::OnEnter()
{
	TestBullet* b = Instantiate<TestBullet>(pPlayer_->GetParent());
	b->Shot(pPlayer_->GetPosition(), XMFLOAT3(0.0f, 0.0f, 0.0f));
	atkTime_ = 60;
}
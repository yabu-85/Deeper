#include "PlayerState.h"
#include "Player.h"
#include "StateManager.h"
#include "PlayerCommand.h"

namespace {
	const int defAvoTime = 30;
	const float defAvoSpeed = 2.0f;
}

void PlayerWait::Update()
{
	//�L�[���͂ŃX�e�[�g�؂�ւ�
	if (pPlayer_->GetCommand()->CmdWalk()) {
		owner_->ChangeState("Walk");
		return;
	}
	if (pPlayer_->GetCommand()->CmdAvo()) {
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

	if (!pPlayer_->GetCommand()->CmdWalk()) {
		owner_->ChangeState("Wait");
		return;
	}
	if (pPlayer_->GetCommand()->CmdAvo()) {
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

	const float endValue = 0.1f; //�P�D�O�`���̒l�K�Ƃ��悤�ɂ���
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

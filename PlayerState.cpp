#include "PlayerState.h"
#include "Player.h"

void PlayerWait::OnEnter()
{
}

void PlayerWait::Update()
{
	Player* pPlayer = static_cast<Player*>(owner_);

	//キー入力でステート切り替え
	if (pPlayer->IsMoveKeyPushed())
	{
		//mOwnerCompo->ChangeState("Walk");
		return;
	}

}

//------------------------------------------------------------

void PlayerWalk::OnEnter()
{
}

void PlayerWalk::Update()
{
	Player* pPlayer = static_cast<Player*>(owner_);
	pPlayer->CalcMove();
	pPlayer->Move();
}

//------------------------------------------------------------

void PlayerAvo::Update()
{
	Player* pPlayer = static_cast<Player*>(owner_);
	pPlayer->SetRotateY(pPlayer->GetRotate().y + 10.0f);

}

void PlayerAvo::OnEnter()
{
}

void PlayerAvo::OnExit()
{
}

#include "FeetState.h"
#include "StateManager.h"
#include "MoveActionNode.h"
#include "BehaviourNode.h"
#include "PlayerConditionNode.h"
#include "IsEnemyStateNode.h"
#include "Player.h"
#include "FeetNode.h"
#include "Feet.h"
#include "DamageCtrl.h"
#include "GameManager.h"

FeetAppear::FeetAppear(StateManager* owner) : time_(0), appearTime_(0)
{
	owner_ = owner;
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetAppear::Update()
{
	time_++;
	if (time_ > appearTime_) owner_->ChangeState("Idle");

}

void FeetAppear::Initialize()
{
	appearTime_ = 60;
}

//--------------------------------------------------------------------------------

FeetIdle::FeetIdle(StateManager* owner)
{
	owner_ = owner;
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetIdle::Update()
{
	owner_->ChangeState("Combat");
}

//--------------------------------------------------------------------------------

FeetPatrol::FeetPatrol(StateManager* owner)
{
	owner_ = owner;
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetPatrol::Update()
{
}

//--------------------------------------------------------------------------------

FeetCombat::FeetCombat(StateManager* owner)
{
	owner_ = owner;
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
	Player* pPlayer = static_cast<Player*>(pFeet_->FindObject("Player"));

	//-----------------------------�r�w�C�r�A�c���[�̐ݒ�--------------------------------------
	root_ = new Root();
	Selector* selector1 = new Selector();
	root_->SetRootNode(selector1);

	//---------------------------------------�U���\------------------------------------------------
	Selector* selector2 = new Selector();
	IsNotEnemyCombatState* condition1 = new IsNotEnemyCombatState(selector2, "Attack", pFeet_);
	Inverter* inverter1 = new Inverter(condition1);
	selector1->AddChildren(inverter1);
	
	//--------------------�v���C���[�̋߂��ł͂Ȃ��i�ړ��j/�@�߂��Ȃ�U����I��-----------------------
	MoveTarget* action1 = new MoveTarget(pFeet_);
	IsPlayerNotInRangeNode* condition2 = new IsPlayerNotInRangeNode(action1, 5.0f, pFeet_, pPlayer);
	selector2->AddChildren(condition2);

	Selector* selector3 = new Selector();
	selector2->AddChildren(selector3);

	//--------------------------------�U���̂ǂꂩ��I��-------------------------------------------
	FeetNormalAttack* action2 = new FeetNormalAttack(pFeet_);
	selector3->AddChildren(action2);

	//--------------------------------�ݒ�I���-------------------------------------------

}

void FeetCombat::Update()
{
	root_->Update();
	pFeet_->GetCombatStateManager()->Update();

}

FeetCombat::~FeetCombat()
{
	delete root_;
}

//--------------------------------------------------------------------------------

FeetDead::FeetDead(StateManager* owner)
{
	owner_ = owner;
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetDead::Update()
{
}

//-------------------------------------CombatState-------------------------------------------

FeetWait::FeetWait(StateManager* owner)
{
	owner_ = owner;
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetWait::Update()
{
}

//--------------------------------------------------------------------------------

#include "MoveAction.h"
#include "RotateAction.h"

FeetMove::FeetMove(StateManager* owner)
{
	owner_ = owner;
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetMove::Update()
{
	if (pFeet_->GetMoveAction()->IsInRange()) {
		Player* pPlayer = (Player*)pFeet_->FindObject("Player");
		pFeet_->GetMoveAction()->SetTarget(pPlayer->GetPosition());
	}
	pFeet_->GetMoveAction()->Update();

	pFeet_->GetRotateAction()->Update();
}

//--------------------------------------------------------------------------------

class ActionAttack;

FeetAttack::FeetAttack(StateManager* owner) : time_(0)
{
	owner_ = owner;
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetAttack::Update()
{
	time_--;

	if (time_ > 0 + 30) {
		float random = time_ / 60.0f + 1.0f;
		pFeet_->SetScale(XMFLOAT3(random * random, pFeet_->GetScale().y, random * random));

		GameManager::GetDamageCtrl()->CalcPlyaer(pFeet_->GetSphereCollider(), 1);
	}
	
	if (time_ <= 0) {
		pFeet_->SetScale(XMFLOAT3(5.0f, 5.0f, 5.0f));
		owner_->ChangeState("Wait");
		return;
	}

}

void FeetAttack::OnEnter()
{
	time_ = 90;
}

//--------------------------------------------------------------------------------

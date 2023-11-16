#include "FeetState.h"
#include "StateManager.h"
#include "Feet.h"

namespace {
	int appearTime = 60;
}

FeetAppear::FeetAppear(StateManager* owner) : time_(0)
{
	owner_ = owner;
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetAppear::Update()
{
	time_++;
	if (time_ > appearTime) owner_->ChangeState("Idle");

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

#include "MoveActionNode.h"
#include "BehaviourNode.h"
#include "TargetConditionCountNode.h"

FeetCombat::FeetCombat(StateManager* owner)
{
	owner_ = owner;
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());

	//ビヘイビアツリーの設定
	root_ = new Root();
	Sequence* seq1 = new Sequence();
	MoveTarget* action1 = new MoveTarget(pFeet_, 0.1f, 2.0f);
	CombatEnemyCount* condition1 = new CombatEnemyCount(3, action1);
	Inverter* inv1 = new Inverter(condition1);
	Succeeder* suc1 = new Succeeder(inv1);
	root_->SetRootNode(seq1);
	seq1->AddChildren(suc1);

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

#include "ActionMove.h"

FeetMove::FeetMove(StateManager* owner)
{
	owner_ = owner;
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetMove::Update()
{
	pFeet_->GetActionMove()->Update();
}

//--------------------------------------------------------------------------------

FeetAttack::FeetAttack(StateManager* owner)
{
	owner_ = owner;
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetAttack::Update()
{
}

//--------------------------------------------------------------------------------

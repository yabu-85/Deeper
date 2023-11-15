#include "FeetState.h"
#include "StateManager.h"
#include "Feet.h"

FeetAppear::FeetAppear(StateManager* owner)
{
	owner_ = owner;
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetAppear::Update()
{
}

//--------------------------------------------------------------------------------

FeetIdle::FeetIdle(StateManager* owner)
{
	owner_ = owner;
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetIdle::Update()
{
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

#include "MoveAction.h"
#include "BehaviourNode.h"
#include "TargetConditionCount.h"
#include "BaseAction.h"

FeetCombat::FeetCombat(StateManager* owner)
{
	owner_ = owner;
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
	pCombatStateManager_ = new StateManager(pFeet_);

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
	pCombatStateManager_->Update();

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

//--------------------------------------------------------------------------------

FeetMove::FeetMove(StateManager* owner)
{
	owner_ = owner;
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetMove::Update()
{
}

//--------------------------------------------------------------------------------


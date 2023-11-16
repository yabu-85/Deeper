#include "FeetState.h"
#include "StateManager.h"
#include "Feet.h"

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

#include "MoveActionNode.h"
#include "BehaviourNode.h"
#include "TargetConditionCountNode.h"
#include "PlayerConditionNode.h"
#include "Player.h"

FeetCombat::FeetCombat(StateManager* owner)
{
	owner_ = owner;
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
	Player* pPlayer = static_cast<Player*>(pFeet_->FindObject("Player"));

	//ビヘイビアツリーの設定
	root_ = new Root();
	Selector* selector1 = new Selector();
	root_->SetRootNode(selector1);			//rootを設定

	Selector* selector2 = new Selector();
	Succeeder* succeeder1 = new Succeeder(selector2);
	selector1->AddChildren(succeeder1);		//攻撃可能だったら	eeeeeeeeeeee

	MoveTarget* action1 = new MoveTarget(pFeet_);
	IsPlayerInRangeNode* condition1 = new IsPlayerInRangeNode(10.0f, action1, pFeet_, pPlayer);
	Inverter* inverter1 = new Inverter(condition1);
	selector2->AddChildren(inverter1);		//プレイヤーの近くにいないなら移動する

	Selector* selector3 = new Selector();
	selector2->AddChildren(selector3);		//近くにいるから攻撃のどれかを選択する	eeeeeeeeeeeee

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
	pFeet_->GetMoveAction()->Update();
	pFeet_->GetRotateAction()->Update();
}

//--------------------------------------------------------------------------------

class ActionAttack;

FeetAttack::FeetAttack(StateManager* owner)
{
	owner_ = owner;
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetAttack::Update()
{
}

//--------------------------------------------------------------------------------

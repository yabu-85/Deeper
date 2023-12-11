#include "FeetState.h"
#include "StateManager.h"
#include "ChangeStateNode.h"
#include "BehaviourNode.h"
#include "PlayerConditionNode.h"
#include "IsEnemyStateNode.h"
#include "Player.h"
#include "Feet.h"
#include "Stage.h"
#include "DamageCtrl.h"
#include "GameManager.h"
#include "EnemyUi.h"

#include "MoveAction.h"
#include "RotateAction.h"
#include "Engine/Model.h"

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
	owner_->ChangeState("Patrol");
}

//--------------------------------------------------------------------------------

FeetPatrol::FeetPatrol(StateManager* owner)
{
	owner_ = owner;
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetPatrol::Update()
{
	if (pFeet_->GetMoveAction()->IsInRange() && rand() % 60 == 0) {
		Stage* pStage = (Stage*)pFeet_->FindObject("Stage");
		pFeet_->GetMoveAction()->SetTarget(pStage->GetRandomFloarPosition());
	}
	pFeet_->GetMoveAction()->Update();
	pFeet_->GetRotateAction()->Update();

	Player* pPlayer = (Player*)pFeet_->FindObject("Player");
	XMFLOAT3 pos = pPlayer->GetPosition();
	XMFLOAT3 tar = pFeet_->GetPosition();
	XMVECTOR vPos = XMLoadFloat3(&pos);
	XMVECTOR vTar = XMLoadFloat3(&tar);
	float range = XMVectorGetX(XMVector3Length(vTar - vPos));
	if(range <= 10.0f) owner_->ChangeState("Combat");

}

void FeetPatrol::OnEnter()
{
	pFeet_->GetMoveAction()->SetMoveSpeed(0.03f);
	pFeet_->GetRotateAction()->SetTarget(nullptr);
}

void FeetPatrol::OnExit()
{
	pFeet_->GetMoveAction()->SetMoveSpeed(0.07f);
	pFeet_->GetMoveAction()->StopMove();

}

//--------------------------------------------------------------------------------

FeetCombat::FeetCombat(StateManager* owner)
{
	owner_ = owner;
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
	Player* pPlayer = static_cast<Player*>(pFeet_->FindObject("Player"));

	//-----------------------------ビヘイビアツリーの設定--------------------------------------
	root_ = new Root();
	Selector* selector1 = new Selector();
	root_->SetRootNode(selector1);

	//---------------------------------------攻撃可能------------------------------------------------
	Selector* selector2 = new Selector();
	IsNotEnemyCombatState* condition1 = new IsNotEnemyCombatState(selector2, "Attack", pFeet_);
	Inverter* inverter1 = new Inverter(condition1);
	selector1->AddChildren(inverter1);
	
	//--------------------プレイヤーの近くではない（移動）/　近くなら攻撃を選ぶ-----------------------
	EnemyChangeCombatStateNode* action1 = new EnemyChangeCombatStateNode(pFeet_, "Move");
	IsPlayerNotInRangeNode* condition2 = new IsPlayerNotInRangeNode(action1, 5.0f, pFeet_, pPlayer);
	selector2->AddChildren(condition2);

	Selector* selector3 = new Selector();
	selector2->AddChildren(selector3);

	//--------------------------------攻撃のどれかを選ぶ-------------------------------------------
	EnemyChangeCombatStateNode* action2 = new EnemyChangeCombatStateNode(pFeet_, "Attack");
	selector3->AddChildren(action2);

}

void FeetCombat::Update()
{
	root_->Update();
	pFeet_->GetCombatStateManager()->Update();

}

void FeetCombat::OnEnter()
{
	pFeet_->GetEnemyUi()->InitTargetFoundUi();
	pFeet_->GetRotateAction()->Initialize();

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

void FeetMove::OnExit()
{
	pFeet_->GetMoveAction()->StopMove();
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
	time_++;

	//AttackFrame=65 ～ 90
	if (time_ > 65 && time_ < 90) {
		GameManager::GetDamageCtrl()->CalcPlyaer(pFeet_->GetSphereCollider(), 1);
	}
	
	if (time_ >= 200) {
		Model::SetAnimFrame(pFeet_->GetModelHandle(), 0, 0, 1.0f);
		owner_->ChangeState("Wait");
		return;
	}
}

void FeetAttack::OnEnter()
{
	time_ = 0;
	Model::SetAnimFrame(pFeet_->GetModelHandle(), 0, 200, 1.0f);
}

//--------------------------------------------------------------------------------

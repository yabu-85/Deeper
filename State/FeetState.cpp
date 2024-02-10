#include "FeetState.h"
#include "StateManager.h"
#include "../Player/Player.h"
#include "../Enemy/Feet.h"
#include "../Stage/CreateStage.h"
#include "../GameManager.h"
#include "../Enemy/EnemyUi.h"
#include "../Engine/Model.h"
#include "../VFXManager.h"

#include "../BehaviorTree/IsEnemyActionReadyNode.h"
#include "../BehaviorTree/ChangeStateNode.h"
#include "../BehaviorTree/BehaviourNode.h"
#include "../BehaviorTree/PlayerConditionNode.h"
#include "../BehaviorTree/StateCountNode.h"
#include "../BehaviorTree/IsEnemyStateNode.h"
#include "../Action/MoveAction.h"
#include "../Action/RotateAction.h"
#include "../Action/SearchAction.h"

namespace {
	static const int FOUND_SEARCH = 10;		//視覚探知の更新時間
	static const int APPER_TIME = 180;
	static const float FAST_SPEED = 0.08f;
	static const float SLOW_SPEED = 0.06f;

}

FeetAppear::FeetAppear(StateManager* owner) : StateBase(owner), time_(0)
{
}

void FeetAppear::Update()
{
	time_++;
	if (time_ > APPER_TIME) owner_->ChangeState("Patrol");

	float tsize = (float)time_ / (float)APPER_TIME;
	Feet* f = static_cast<Feet*>(owner_->GetGameObject());
	f->SetScale(XMFLOAT3(tsize, tsize, tsize));

}

void FeetAppear::OnEnter()
{
	Feet* f = static_cast<Feet*>(owner_->GetGameObject());
	XMFLOAT3 pos = f->GetPosition();
	VFXManager::CreatVfxEnemySpawn(pos, APPER_TIME);

}

void FeetAppear::OnExit()
{
	Feet* f = static_cast<Feet*>(owner_->GetGameObject());
	f->SetScale(XMFLOAT3(1, 1, 1));

}

//--------------------------------------------------------------------------------

FeetPatrol::FeetPatrol(StateManager* owner) : StateBase(owner), foundSearchTime_(0)
{
}

void FeetPatrol::Update()
{
	//Astar移動が終わったなら更新・待ち時間適当にrandamで デバッグ用
	Feet* f = static_cast<Feet*>(owner_->GetGameObject());
	if (f->GetMoveAction()->IsInRange() && rand() % 60 == 0) {
		CreateStage* pCreateStage = GameManager::GetCreateStage();
		f->GetMoveAction()->UpdatePath(pCreateStage->GetRandomFloarPosition());
	}

	//Astar移動・回転
	f->GetMoveAction()->Update();
	f->GetRotateAction()->Update();

	//FoundSearchの実行待ち時間がfoundSearch
	foundSearchTime_++;
	if (foundSearchTime_ > FOUND_SEARCH) {
		foundSearchTime_ = 0;
		f->GetVisionSearchAction()->Update();

		//見つかったらCombatStateへ推移
		if (f->GetVisionSearchAction()->IsFoundTarget()) {
			owner_->ChangeState("Combat");
		}
	}
}

void FeetPatrol::OnEnter()
{
	Feet* f = static_cast<Feet*>(owner_->GetGameObject());
	f->GetMoveAction()->SetMoveSpeed(SLOW_SPEED);
	f->GetRotateAction()->SetTarget(nullptr);
}

void FeetPatrol::OnExit()
{
	Feet* f = static_cast<Feet*>(owner_->GetGameObject());
	f->GetMoveAction()->SetMoveSpeed(FAST_SPEED);
	f->GetMoveAction()->StopMove();
}

//--------------------------------------------------------------------------------

FeetCombat::FeetCombat(StateManager* owner) : StateBase(owner)
{
	Feet* f = static_cast<Feet*>(owner_->GetGameObject());
	
	//-----------------------------ビヘイビアツリーの設定--------------------------------------
	root_ = new Root();
	Selector* selector1 = new Selector();
	root_->SetRootNode(selector1);

	//---------------------------------------攻撃StateのSelectorの登録----------------------------
	Selector* selector2 = new Selector();
	IsNotEnemyCombatState* condition1 = new IsNotEnemyCombatState(selector2, "Attack", f);
	selector1->AddChildren(condition1);

	//--------------------Moveへ移行する------------------------------------
	EnemyChangeCombatStateNode* action1 = new EnemyChangeCombatStateNode(f, "Move");
	IsEnemyAttackReady* condition2 = new IsEnemyAttackReady(action1, f);

	//制御AIのConditionNode（攻撃可能最大数範囲内かTest
	CombatStateCountNode* conditionA = new CombatStateCountNode(condition2, 3, { "Move", "Attack" });
	IsEnemyCombatState* condition3 = new IsEnemyCombatState(conditionA, "Wait", f);
	selector2->AddChildren(condition3);

	//--------------------Attackへ移行する-----------------------
	EnemyChangeCombatStateNode* action2 = new EnemyChangeCombatStateNode(f, "Attack");
	IsPlayerInRangeNode* condition4 = new IsPlayerInRangeNode(action2, 2.0f, f, GameManager::GetPlayer());
	IsEnemyCombatState* condition5 = new IsEnemyCombatState(condition4, "Move", f);
	selector2->AddChildren(condition5);

}

void FeetCombat::Update()
{
	root_->Update();
	Feet* f = static_cast<Feet*>(owner_->GetGameObject());
	f->GetCombatStateManager()->Update();

}

void FeetCombat::OnEnter()
{
	Feet* f = static_cast<Feet*>(owner_->GetGameObject());
	f->GetEnemyUi()->InitTargetFoundUi();
	f->GetRotateAction()->Initialize();
	f->GetRotateAction()->SetTarget(GameManager::GetPlayer());

}

FeetCombat::~FeetCombat()
{
	delete root_;
}

//--------------------------------------------------------------------------------

FeetDead::FeetDead(StateManager* owner) : StateBase(owner)
{
}

void FeetDead::Update()
{
}

//-------------------------------------CombatState-------------------------------------------

FeetWait::FeetWait(StateManager* owner) : StateBase(owner)
{
}

void FeetWait::Update()
{
	Feet* f = static_cast<Feet*>(owner_->GetGameObject());
	f->GetRotateAction()->Update();

}

void FeetWait::OnEnter()
{
	Feet* f = static_cast<Feet*>(owner_->GetGameObject());
	f->GetMoveAction()->SetMoveSpeed(SLOW_SPEED);
}

//--------------------------------------------------------------------------------

FeetMove::FeetMove(StateManager* owner) : StateBase(owner)
{
}

void FeetMove::Update()
{
	//らんｄやめよう
	Feet* f = static_cast<Feet*>(owner_->GetGameObject());
	f->GetMoveAction()->SetTarget(GameManager::GetPlayer()->GetPosition());
	if (f->GetMoveAction()->IsInRange() && rand() % 10 == 0) {
		f->GetMoveAction()->UpdatePath(GameManager::GetPlayer()->GetPosition());
	}

	if (f->GetMoveAction()->IsOutTarget(3.0f)) {
		f->GetMoveAction()->UpdatePath(GameManager::GetPlayer()->GetPosition());
	}

	f->GetMoveAction()->Update();
	f->GetRotateAction()->Update();
}

void FeetMove::OnEnter()
{
	Feet* f = static_cast<Feet*>(owner_->GetGameObject());
	f->GetMoveAction()->SetMoveSpeed(FAST_SPEED);

}

void FeetMove::OnExit()
{
	Feet* f = static_cast<Feet*>(owner_->GetGameObject());
	f->GetMoveAction()->StopMove();
}

//--------------------------------------------------------------------------------

FeetAttack::FeetAttack(StateManager* owner) : StateBase(owner), time_(0)
{
}

void FeetAttack::Update()
{
	time_++;

	Feet* f = static_cast<Feet*>(owner_->GetGameObject());
	if (time_ < 30) f->GetRotateAction()->Update();

	//AttackFrame=65 ～ 90
	//FeetのOnAttackCollisionの方でPlayerに当たったらfalseにする処理を書いてある
	if (time_ == 65) f->GetSphereCollider()->SetValid(true);
	if (time_ == 90) f->GetSphereCollider()->SetValid(false);

	if (time_ >= 200) {
		Model::SetAnimFrame(f->GetModelHandle(), 0, 0, 1.0f);
		owner_->ChangeState("Wait");
		return;
	}
}

void FeetAttack::OnEnter()
{
	time_ = 0;
	Feet* f = static_cast<Feet*>(owner_->GetGameObject());
	Model::SetAnimFrame(f->GetModelHandle(), 0, 200, 1.0f);
}

//--------------------------------------------------------------------------------

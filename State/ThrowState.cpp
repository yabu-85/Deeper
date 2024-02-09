#include "ThrowState.h"
#include "StateManager.h"
#include "../Player/Player.h"
#include "../Enemy/ThrowEnemy.h"
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

ThrowAppear::ThrowAppear(StateManager* owner) : StateBase(owner), time_(0)
{
}

void ThrowAppear::Update()
{
	time_++;
	if (time_ > APPER_TIME) owner_->ChangeState("Idle");

	float tsize = (float)time_ / (float)APPER_TIME * 0.5f;
	ThrowEnemy* f = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	f->SetScale(XMFLOAT3(tsize, tsize, tsize));

}

void ThrowAppear::OnEnter()
{
	ThrowEnemy* f = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	XMFLOAT3 pos = f->GetPosition();
	VFXManager::CreatVfxEnemySpawn(pos, APPER_TIME);

}

void ThrowAppear::OnExit()
{
	ThrowEnemy* f = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	f->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));

}

//--------------------------------------------------------------------------------

ThrowIdle::ThrowIdle(StateManager* owner) : StateBase(owner)
{
}

void ThrowIdle::Update()
{
	owner_->ChangeState("Patrol");
}

//--------------------------------------------------------------------------------

ThrowPatrol::ThrowPatrol(StateManager* owner) : StateBase(owner), foundSearchTime_(0)
{
}

void ThrowPatrol::Update()
{
	//Astar移動が終わったなら更新・待ち時間適当にrandamで デバッグ用
	ThrowEnemy* f = static_cast<ThrowEnemy*>(owner_->GetGameObject());
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

void ThrowPatrol::OnEnter()
{
	ThrowEnemy* f = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	f->GetMoveAction()->SetMoveSpeed(SLOW_SPEED);
	f->GetRotateAction()->SetTarget(nullptr);
}

void ThrowPatrol::OnExit()
{
	ThrowEnemy* f = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	f->GetMoveAction()->SetMoveSpeed(FAST_SPEED);
	f->GetMoveAction()->StopMove();
}

//--------------------------------------------------------------------------------

ThrowCombat::ThrowCombat(StateManager* owner) : StateBase(owner)
{
	ThrowEnemy* f = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	
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
	CombatStateCountNode* conditionA = new CombatStateCountNode(condition2, 2, { "Move", "Attack" });
	IsEnemyCombatState* condition3 = new IsEnemyCombatState(conditionA, "Wait", f);
	selector2->AddChildren(condition3);

	//--------------------Attackへ移行する-----------------------
	EnemyChangeCombatStateNode* action2 = new EnemyChangeCombatStateNode(f, "Attack");
	IsPlayerInRangeNode* condition4 = new IsPlayerInRangeNode(action2, 2.0f, f, GameManager::GetPlayer());
	IsEnemyCombatState* condition5 = new IsEnemyCombatState(condition4, "Move", f);
	selector2->AddChildren(condition5);

}

void ThrowCombat::Update()
{
	root_->Update();
	ThrowEnemy* f = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	f->GetCombatStateManager()->Update();

}

void ThrowCombat::OnEnter()
{
	ThrowEnemy* f = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	f->GetEnemyUi()->InitTargetFoundUi();
	f->GetRotateAction()->Initialize();
	f->GetRotateAction()->SetTarget(GameManager::GetPlayer());

}

ThrowCombat::~ThrowCombat()
{
	delete root_;
}

//--------------------------------------------------------------------------------

ThrowDead::ThrowDead(StateManager* owner) : StateBase(owner)
{
}

void ThrowDead::Update()
{
}

//-------------------------------------CombatState-------------------------------------------

ThrowWait::ThrowWait(StateManager* owner) : StateBase(owner)
{
}

void ThrowWait::Update()
{
	ThrowEnemy* f = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	if (rand() % 100 == 0) {
		if (f->GetMoveAction()->IsInRange() || f->GetMoveAction()->IsOutTarget(3.0f)) {
			CreateStage* pCreateStage = GameManager::GetCreateStage();
			f->GetMoveAction()->UpdatePath(pCreateStage->GetFloarPosition(f->GetPosition(), 10.0f));
		}
	}

	f->GetMoveAction()->Update();
	f->GetRotateAction()->Update();

}

void ThrowWait::OnEnter()
{
	ThrowEnemy* f = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	f->GetMoveAction()->SetMoveSpeed(SLOW_SPEED);
}

//--------------------------------------------------------------------------------

ThrowMove::ThrowMove(StateManager* owner) : StateBase(owner)
{
}

void ThrowMove::Update()
{
	//らんｄやめよう
	ThrowEnemy* f = static_cast<ThrowEnemy*>(owner_->GetGameObject());
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

void ThrowMove::OnEnter()
{
	ThrowEnemy* f = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	f->GetMoveAction()->SetMoveSpeed(FAST_SPEED);

}

void ThrowMove::OnExit()
{
	ThrowEnemy* f = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	f->GetMoveAction()->StopMove();
}

//--------------------------------------------------------------------------------

ThrowAttack::ThrowAttack(StateManager* owner) : StateBase(owner), time_(0)
{
}

void ThrowAttack::Update()
{
	time_++;

	ThrowEnemy* f = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	if (time_ < 30) f->GetRotateAction()->Update();

	//攻撃するー
	//if (time_ == 65) f->GetSphereCollider()->SetValid(true);

	if (time_ >= 200) {
		Model::SetAnimFrame(f->GetModelHandle(), 0, 0, 1.0f);
		owner_->ChangeState("Wait");
		return;
	}
}

void ThrowAttack::OnEnter()
{
	time_ = 0;
	ThrowEnemy* f = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	Model::SetAnimFrame(f->GetModelHandle(), 0, 200, 1.0f);
}

//--------------------------------------------------------------------------------

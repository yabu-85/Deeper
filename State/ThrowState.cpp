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
#include "../BehaviorTree/IsEnemyStateNode.h"
#include "../Action/MoveAction.h"
#include "../Action/RotateAction.h"
#include "../Action/SearchAction.h"
#include "../BehaviorTree/IsEnemyAttackPermission.h"

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
	if (time_ > APPER_TIME) owner_->ChangeState("Patrol");

	float tsize = (float)time_ / (float)APPER_TIME * 0.5f;
	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	e->SetScale(XMFLOAT3(tsize, tsize, tsize));

}

void ThrowAppear::OnEnter()
{
	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	XMFLOAT3 pos = e->GetPosition();
	VFXManager::CreatVfxEnemySpawn(pos, APPER_TIME);

}

void ThrowAppear::OnExit()
{
	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	e->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));

}

//--------------------------------------------------------------------------------

ThrowDead::ThrowDead(StateManager* owner) : StateBase(owner)
{
}

void ThrowDead::Update()
{
}

void ThrowDead::OnEnter()
{
	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	e->Dead();
}

//--------------------------------------------------------------------------------

ThrowPatrol::ThrowPatrol(StateManager* owner) : StateBase(owner), foundSearchTime_(0)
{
}

void ThrowPatrol::Update()
{
	//Astar移動が終わったなら更新・待ち時間適当にrandamで デバッグ用
	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	if (e->GetMoveAction()->IsInRange() && rand() % 60 == 0) {
		CreateStage* pCreateStage = GameManager::GetCreateStage();
		e->GetMoveAction()->UpdatePath(pCreateStage->GetRandomFloarPosition());
	}

	//Astar移動・回転
	e->GetMoveAction()->Update();
	e->GetRotateAction()->Update();

	//FoundSearchの実行待ち時間がfoundSearch
	foundSearchTime_++;
	if (foundSearchTime_ > FOUND_SEARCH) {
		foundSearchTime_ = 0;
		e->GetVisionSearchAction()->Update();

		//見つかったらCombatStateへ推移
		if (e->GetVisionSearchAction()->IsFoundTarget()) {
			owner_->ChangeState("Combat");
		}
	}
}

void ThrowPatrol::OnEnter()
{
	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	e->GetMoveAction()->SetMoveSpeed(SLOW_SPEED);
	e->GetRotateAction()->SetTarget(nullptr);
}

void ThrowPatrol::OnExit()
{
	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	e->GetMoveAction()->SetMoveSpeed(FAST_SPEED);
	e->GetMoveAction()->StopMove();
}

//--------------------------------------------------------------------------------

ThrowCombat::ThrowCombat(StateManager* owner) : StateBase(owner), time_(0)
{
	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	
	//-----------------------------ビヘイビアツリーの設定--------------------------------------
	root_ = new Root();
	Selector* selector1 = new Selector();
	root_->SetRootNode(selector1);

	//---------------------------------------攻撃StateのSelectorの登録----------------------------
	Selector* selector2 = new Selector();
	IsNotEnemyCombatState* condition1 = new IsNotEnemyCombatState(selector2, "Attack", e);
	selector1->AddChildren(condition1);

	//--------------------Moveへ移行する------------------------------------
	EnemyChangeCombatStateNode* action1 = new EnemyChangeCombatStateNode(e, "Move");
	IsEnemyAttackReady* condition2 = new IsEnemyAttackReady(action1, e);

	//制御AIのConditionNode（攻撃可能最大数範囲内かTest
	IsEnemyAttackPermission* conditionA = new IsEnemyAttackPermission(condition2, e);
	IsEnemyCombatState* condition3 = new IsEnemyCombatState(conditionA, "Wait", e);
	selector2->AddChildren(condition3);

	//--------------------Attackへ移行する-----------------------
	EnemyChangeCombatStateNode* action2 = new EnemyChangeCombatStateNode(e, "Attack");
	IsPlayerInRangeNode* condition4 = new IsPlayerInRangeNode(action2, 6.0f, e, GameManager::GetPlayer());
	IsEnemyCombatState* condition5 = new IsEnemyCombatState(condition4, "Move", e);
	selector2->AddChildren(condition5);

}

void ThrowCombat::Update()
{
	time_++;
	if(time_ % 10 == 0) root_->Update();
	
	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	e->GetCombatStateManager()->Update();

}

void ThrowCombat::OnEnter()
{
	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	e->GetEnemyUi()->InitTargetFoundUi();
	e->GetRotateAction()->Initialize();
	e->GetRotateAction()->SetTarget(GameManager::GetPlayer());

}

ThrowCombat::~ThrowCombat()
{
	delete root_;
}

//-------------------------------------CombatState-------------------------------------------

ThrowWait::ThrowWait(StateManager* owner) : StateBase(owner)
{
}

void ThrowWait::Update()
{
	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());

	//Playerが特定の距離移動したら
	if (rand() % 30 == 0) {
		e->GetMoveAction()->SetTarget(GameManager::GetPlayer()->GetPosition());
		if (e->GetMoveAction()->IsOutTarget(5.0f)) {
			CreateStage* pCreateStage = GameManager::GetCreateStage();
			e->GetMoveAction()->UpdatePath(pCreateStage->GetFloarPosition(e->GetPosition(), 7.0f));
		}
	}

	e->GetMoveAction()->Update();
	e->GetRotateAction()->Update();
}

void ThrowWait::OnEnter()
{
	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	e->GetMoveAction()->SetMoveSpeed(SLOW_SPEED);
	e->SetItem();

}

//--------------------------------------------------------------------------------

ThrowMove::ThrowMove(StateManager* owner) : StateBase(owner)
{
}

void ThrowMove::Update()
{
	//プレイヤーの場所に移動
	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	e->GetMoveAction()->SetTarget(GameManager::GetPlayer()->GetPosition());
	if (e->GetMoveAction()->IsOutTarget(3.0f)) {
		e->GetMoveAction()->UpdatePath(GameManager::GetPlayer()->GetPosition());
	}

	e->GetMoveAction()->Update();
	e->GetRotateAction()->Update();
}

void ThrowMove::OnEnter()
{
	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	e->GetMoveAction()->SetMoveSpeed(FAST_SPEED);

}

void ThrowMove::OnExit()
{
	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	e->GetMoveAction()->StopMove();
}

//--------------------------------------------------------------------------------

ThrowAttack::ThrowAttack(StateManager* owner) : StateBase(owner), time_(0)
{
}

void ThrowAttack::Update()
{
	time_++;

	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	e->GetRotateAction()->Update();
	
	if(time_ == 65) e->ThrowItem();

	if (time_ >= 200) {
		Model::SetAnimFrame(e->GetModelHandle(), 0, 0, 1.0f);
		owner_->ChangeState("Wait");
		return;
	}
}

void ThrowAttack::OnEnter()
{
	time_ = 0;
	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	Model::SetAnimFrame(e->GetModelHandle(), 0, 200, 1.0f);
	e->SetAttackCoolDown(200 + rand() % 100);

}

//--------------------------------------------------------------------------------

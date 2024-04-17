#include "MeleeFighterState.h"
#include "StateManager.h"
#include "../Player/Player.h"
#include "../Player/Aim.h"
#include "../Enemy/MeleeFighter.h"
#include "../Stage/CreateStage.h"
#include "../GameManager/GameManager.h"
#include "../Enemy/EnemyUi.h"
#include "../Engine/Model.h"
#include "../VFXManager.h"

#include "../BehaviorTree/IsEnemyActionReadyNode.h"
#include "../BehaviorTree/ChangeStateNode.h"
#include "../BehaviorTree/BehaviourNode.h"
#include "../BehaviorTree/PlayerConditionNode.h"
#include "../BehaviorTree/IsEnemyStateNode.h"
#include "../BehaviorTree/IsEnemyPermission.h"

#include "../Action/MoveAction.h"
#include "../Action/RotateAction.h"
#include "../Action/SearchAction.h"

namespace {
	static const int APPER_TIME = 180;
	static const int DEAD_TIME = 100;
	static const int FOUND_SEARCH = 10;
	static const int FPS = 60;
	static const int MIN_MOVE_TIME = 6;
	static const int MAX_MOVE_TIME = 5; 

	static const float FAST_SPEED = 0.06f;
	static const float SLOW_SPEED = 0.04f;
	static const float ROTATE_RATIO = 0.07f;

	//攻撃１の情報
	static const int ROTATE_FRAME = 45;
	static const float ATTACK_READY_DISTANCE = 2.0f;

}

MeleeFighterAppear::MeleeFighterAppear(StateManager* owner) : StateBase(owner), time_(0)
{
}

void MeleeFighterAppear::Update()
{
	time_++;
	MeleeFighter* e = static_cast<MeleeFighter*>(owner_->GetGameObject());
	
	float tsize = (float)time_ / (float)APPER_TIME;
	e->SetScale(XMFLOAT3(tsize, tsize, tsize));

	if (time_ > APPER_TIME) owner_->ChangeState("Patrol");
}

void MeleeFighterAppear::OnEnter()
{
	MeleeFighter* e = static_cast<MeleeFighter*>(owner_->GetGameObject());
	e->GetAnimationController()->SetNextAnime((int)MELEE_ANIMATION::IDLE, 0.1f);

	XMFLOAT3 pos = e->GetPosition();
	VFXManager::CreatVfxEnemySpawn(pos, APPER_TIME);

}

void MeleeFighterAppear::OnExit()
{
	MeleeFighter* e = static_cast<MeleeFighter*>(owner_->GetGameObject());
	e->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));

}

//--------------------------------------------------------------------------------

MeleeFighterDead::MeleeFighterDead(StateManager* owner) : StateBase(owner), time_(0)
{
}

void MeleeFighterDead::Update()
{
	MeleeFighter* e = static_cast<MeleeFighter*>(owner_->GetGameObject());
	time_++;
	
	float s = (float)time_ / (float)DEAD_TIME;
	s = 1.0f - s;
	e->SetScale({ s, s, s });

	if (time_ >= DEAD_TIME) e->DeadExit();
}

void MeleeFighterDead::OnEnter()
{
	MeleeFighter* e = static_cast<MeleeFighter*>(owner_->GetGameObject());
	e->DeadEnter();
	e->GetAnimationController()->SetNextAnime((int)MELEE_ANIMATION::IDLE, 0.1f);

	time_ = 0;
}

//--------------------------------------------------------------------------------

MeleeFighterPatrol::MeleeFighterPatrol(StateManager* owner) : StateBase(owner), foundSearchTime_(0)
{
}

void MeleeFighterPatrol::Update()
{
	//Astar移動が終わったなら更新・待ち時間適当にrandamで デバッグ用
	MeleeFighter* e = static_cast<MeleeFighter*>(owner_->GetGameObject());
	if (rand() % 60 == 0 && e->GetMoveAction()->IsInRange()) {
		e->GetMoveAction()->UpdatePath(GameManager::GetCreateStage()->GetRandomFloarPosition());
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

void MeleeFighterPatrol::OnEnter()
{
	MeleeFighter* e = static_cast<MeleeFighter*>(owner_->GetGameObject());
	e->GetMoveAction()->SetMoveSpeed(SLOW_SPEED);
	e->GetRotateAction()->SetTarget(nullptr);
	e->GetRotateAction()->SetRatio(ROTATE_RATIO);
	e->GetAnimationController()->SetNextAnime((int)MELEE_ANIMATION::WALK, 0.1f);

}

void MeleeFighterPatrol::OnExit()
{
	MeleeFighter* e = static_cast<MeleeFighter*>(owner_->GetGameObject());
	e->GetMoveAction()->StopMove();
}

//--------------------------------------------------------------------------------

MeleeFighterCombat::MeleeFighterCombat(StateManager* owner) : StateBase(owner), time_(0)
{
	MeleeFighter* e = static_cast<MeleeFighter*>(owner_->GetGameObject());
	
	//----------ビヘイビアツリーの設定------------
	root_ = new Root();

	Selector* selector1 = new Selector();
	root_->SetRootNode(selector1);

	Selector* waitSelector = new Selector();
	Selector* moveSelector = new Selector();
	IsEnemyCombatState* wCon = new IsEnemyCombatState(waitSelector, "Wait", e);
	IsEnemyCombatState* mCon = new IsEnemyCombatState(moveSelector, "Move", e);
	selector1->AddChildren(wCon);
	selector1->AddChildren(mCon);

	//-------------------------------------Wait--------------------------------------
	EnemyChangeCombatStateNode* action3 = new EnemyChangeCombatStateNode(e, "Attack");
	IsEnemyAttackPermission* condition5 = new IsEnemyAttackPermission(action3, e);
	IsPlayerInRangeNode* condition6 = new IsPlayerInRangeNode(condition5, e->GetAttackDistance(), e, GameManager::GetPlayer());
	waitSelector->AddChildren(condition6);

	EnemyChangeCombatStateNode* action1 = new EnemyChangeCombatStateNode(e, "Move");
	IsEnemyMovePermission* condition2 = new IsEnemyMovePermission(action1, e);
	IsEnemyActionReady* condition3 = new IsEnemyActionReady(condition2, e);
	waitSelector->AddChildren(condition3);

	//-------------------------------------Move--------------------------------------
	moveSelector->AddChildren(condition6);

	//-------------------------------------Attack--------------------------------------

}

void MeleeFighterCombat::Update()
{
	time_++;
	if (time_ % 10 == 0) root_->Update();
	
	MeleeFighter* e = static_cast<MeleeFighter*>(owner_->GetGameObject());
	e->GetCombatStateManager()->Update();

}

void MeleeFighterCombat::OnEnter()
{
	MeleeFighter* e = static_cast<MeleeFighter*>(owner_->GetGameObject());
	e->GetEnemyUi()->InitTargetFoundUi();
	e->GetRotateAction()->Initialize();
	e->GetRotateAction()->SetTarget(GameManager::GetPlayer());
	e->GetRotateAction()->SetRatio(ROTATE_RATIO);

}

MeleeFighterCombat::~MeleeFighterCombat()
{
	delete root_;
}

//-------------------------------------CombatState-------------------------------------------

MeleeFighterWait::MeleeFighterWait(StateManager* owner) : StateBase(owner)
{
}

void MeleeFighterWait::Update()
{
	MeleeFighter* e = static_cast<MeleeFighter*>(owner_->GetGameObject());
	e->GetRotateAction()->Update();

	//壁に当たったか調べて
	e->GetOrientedMoveAction()->SetTarget(GameManager::GetPlayer()->GetPosition());
	if (e->GetOrientedMoveAction()->CheckWallCollision(1)) {
		e->GetOrientedMoveAction()->SetDirection(XMVector3Normalize(XMVECTOR{ 0.0f, 0.0f, 1.0f, 0.0f }));
	}
	
	e->GetOrientedMoveAction()->Update();

}

void MeleeFighterWait::OnEnter()
{
	MeleeFighter* e = static_cast<MeleeFighter*>(owner_->GetGameObject());
	e->GetMoveAction()->SetMoveSpeed(SLOW_SPEED);
	e->GetAnimationController()->SetNextAnime((int)MELEE_ANIMATION::IDLE, 0.1f);

	//プレイヤーから指定の範囲内で
	//ゲーム参考にしてから作る
	XMFLOAT3 pPos = GameManager::GetPlayer()->GetPosition();
	XMFLOAT3 ePos = e->GetPosition();
	XMFLOAT3 vec = { pPos.x - ePos.x, 0.0f, pPos.z - ePos.z };
	float dist = sqrt(vec.x * vec.x + vec.z * vec.z);
	if(dist <= e->GetCombatDistance()) e->GetOrientedMoveAction()->SetDirection(XMVECTOR{ 0, 0, 1, 0 });
	else {
		int r = rand() % 2;
		if(r == 0) e->GetOrientedMoveAction()->SetDirection(XMVECTOR{ 1, 0, 0, 0 });
		else if (r == 1) e->GetOrientedMoveAction()->SetDirection(XMVECTOR{ -1, 0, 0, 0 });
	}
}

//------------------------------------Move--------------------------------------------

MeleeFighterMove::MeleeFighterMove(StateManager* owner) : StateBase(owner), time_(0)
{
}

void MeleeFighterMove::Update()
{
	time_--;
	MeleeFighter* e = static_cast<MeleeFighter*>(owner_->GetGameObject());
	e->GetMoveAction()->SetTarget(GameManager::GetPlayer()->GetPosition());

	if (e->GetMoveAction()->IsInRange() || (time_ % 5 == 0 && e->GetMoveAction()->IsOutTarget(3.0f)) ) {
		e->GetMoveAction()->UpdatePath(GameManager::GetPlayer()->GetPosition());
	}

	e->GetMoveAction()->Update();
	e->GetRotateAction()->Update();

	//移動時間終了
	if(time_ <= 0) owner_->ChangeState("Wait");
}

void MeleeFighterMove::OnEnter()
{
	time_ = FPS * MIN_MOVE_TIME + rand() % MAX_MOVE_TIME * FPS;
	MeleeFighter* e = static_cast<MeleeFighter*>(owner_->GetGameObject());
	e->GetMoveAction()->SetMoveSpeed(FAST_SPEED);
	e->GetAnimationController()->SetNextAnime((int)MELEE_ANIMATION::WALK, 0.1f);

}

void MeleeFighterMove::OnExit()
{
	MeleeFighter* e = static_cast<MeleeFighter*>(owner_->GetGameObject());
	e->GetMoveAction()->StopMove();
}

//-------------------------------------Attack-------------------------------------------

MeleeFighterAttack::MeleeFighterAttack(StateManager* owner) : StateBase(owner), time_(0)
{
}

void MeleeFighterAttack::Update()
{
	time_++;
	MeleeFighter* e = static_cast<MeleeFighter*>(owner_->GetGameObject());

	//回転やら移動やら
	if (time_ < ROTATE_FRAME) {
		e->GetRotateAction()->Update();
		e->GetOrientedMoveAction()->SetTarget(GameManager::GetPlayer()->GetPosition());
		e->GetOrientedMoveAction()->Update();
	}

	int startT = e->GetAnimationController()->GetAnim((int)MELEE_ANIMATION::PRE_RUN_ATTACK).startFrame;
	int endT = e->GetAnimationController()->GetAnim((int)MELEE_ANIMATION::PRE_RUN_ATTACK).endFrame;
	int time1 = (endT - startT);
	if (time_ == time1) e->GetAnimationController()->SetNextAnime((int)MELEE_ANIMATION::RUN_ATTACK, 0.1f);

	int startT2 = e->GetAnimationController()->GetAnim((int)MELEE_ANIMATION::RUN_ATTACK).startFrame;
	int endT2 = e->GetAnimationController()->GetAnim((int)MELEE_ANIMATION::RUN_ATTACK).endFrame;
	int time2 = time1 + (endT2 - startT2);

	if (time_ >= time1 && time_ < time1 + 10) {
		e->CalcPoly();
	}

	//攻撃フラグの制御
	if (time_ == time1 + 8) { e->SetDamageInfoCombo1(); }
	else if (time_ == time1 + 10) { e->DamageInfoReset(); }

	//エフェクト
	if(time_ == time1 + 10) {
		XMFLOAT3 pos = e->GetPosition();
		XMFLOAT3 cP = e->GetAttackColliderList().front()->GetCenter();
		pos = { pos.x + cP.x, 0.0f , pos.z + cP.z };
		VFXManager::CreatVfxSmoke(pos);
	}

	if (time_ >= time2) {
		owner_->ChangeState("Wait");
	}
}

void MeleeFighterAttack::OnEnter()
{
	time_ = 0;
	MeleeFighter* e = static_cast<MeleeFighter*>(owner_->GetGameObject());
	e->GetAnimationController()->SetNextAnime((int)MELEE_ANIMATION::PRE_RUN_ATTACK, 0.1f);
	e->GetOrientedMoveAction()->SetDirection(XMVECTOR{ 0, 0, 1, 0 });
	e->SetCombatReady(false);
}

void MeleeFighterAttack::OnExit()
{
	MeleeFighter* e = static_cast<MeleeFighter*>(owner_->GetGameObject());
	e->GetRotateAction()->SetRatio(ROTATE_RATIO);
	e->SetAttackCoolDown(rand() % 100);
	e->DamageInfoReset();
}

//--------------------------------------------------------------------------------
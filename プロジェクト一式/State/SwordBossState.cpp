#include "SwordBossState.h"
#include "StateManager.h"
#include "../Player/Player.h"
#include "../Player/Aim.h"
#include "../Enemy/SwordBoss.h"
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

	/*
	//攻撃Stateの情報
	static const int ATTACK_FRAME[2] = { 0, 260 };
	static const float ATTACK_ROTATE_RATIO = 0.05f;
	static const int CALC_FRAME2[2] = { 120, 140 };
	static const int CALC_FRAME3[2] = { 235, 247 };

	//攻撃３の情報
	static const int ROTATE_FRAME3[2] = { 140, 230 };
	static const int ATTACK_EFFECT_TIME[2] = { 244, 247 };
	static const float MOVESPEED_FRAME3 = 0.03f;
	*/

}

SwordBossAppear::SwordBossAppear(StateManager* owner) : StateBase(owner), time_(0)
{
}

void SwordBossAppear::Update()
{
	time_++;
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	
	float tsize = (float)time_ / (float)APPER_TIME;
	e->SetScale(XMFLOAT3(tsize, tsize, tsize));

	if (time_ > APPER_TIME) owner_->ChangeState("Patrol");
}

void SwordBossAppear::OnEnter()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetAnimationController()->SetNextAnime((int)SWORDBOSS_ANIMATION::IDLE, 0.1f);

	XMFLOAT3 pos = e->GetPosition();
	VFXManager::CreatVfxEnemySpawn(pos, APPER_TIME);

}

void SwordBossAppear::OnExit()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));

}

//--------------------------------------------------------------------------------

SwordBossDead::SwordBossDead(StateManager* owner) : StateBase(owner), time_(0)
{
}

void SwordBossDead::Update()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	time_++;
	
	float s = (float)time_ / (float)DEAD_TIME;
	s = 1.0f - s;
	e->SetScale({ s, s, s });

	if (time_ >= DEAD_TIME) e->DeadExit();
}

void SwordBossDead::OnEnter()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->DeadEnter();
	e->GetAnimationController()->SetNextAnime((int)SWORDBOSS_ANIMATION::IDLE, 0.1f);

	time_ = 0;
}

//--------------------------------------------------------------------------------

SwordBossCombat::SwordBossCombat(StateManager* owner) : StateBase(owner), time_(0)
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	
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
	EnemyChangeCombatStateNode* action2 = new EnemyChangeCombatStateNode(e, "Attack");
	IsEnemyAttackPermission* condition4 = new IsEnemyAttackPermission(action2, e);
	IsPlayerInRangeNode* condition7 = new IsPlayerInRangeNode(condition4, e->GetAttackDistance(), e, GameManager::GetPlayer());
	moveSelector->AddChildren(condition7);

	//-------------------------------------Attack--------------------------------------

}

void SwordBossCombat::Update()
{
	time_++;
	if (time_ % 10 == 0) root_->Update();
	
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetCombatStateManager()->Update();

}

void SwordBossCombat::OnEnter()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetEnemyUi()->InitTargetFoundUi();
	e->GetRotateAction()->Initialize();
	e->GetRotateAction()->SetTarget(GameManager::GetPlayer());
	e->GetRotateAction()->SetRatio(ROTATE_RATIO);

}

SwordBossCombat::~SwordBossCombat()
{
	delete root_;
}

//-------------------------------------CombatState-------------------------------------------

SwordBossWait::SwordBossWait(StateManager* owner) : StateBase(owner)
{
}

void SwordBossWait::Update()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetRotateAction()->Update();

	//壁に当たったか調べて
	e->GetOrientedMoveAction()->SetTarget(GameManager::GetPlayer()->GetPosition());
	if (e->GetOrientedMoveAction()->CheckWallCollision(1)) {
		e->GetOrientedMoveAction()->SetDirection(XMVector3Normalize(XMVECTOR{ 0.0f, 0.0f, 1.0f, 0.0f }));
	}
	
	e->GetOrientedMoveAction()->Update();

}

void SwordBossWait::OnEnter()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetMoveAction()->SetMoveSpeed(SLOW_SPEED);
	e->GetAnimationController()->SetNextAnime((int)SWORDBOSS_ANIMATION::IDLE, 0.1f);

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

SwordBossMove::SwordBossMove(StateManager* owner) : StateBase(owner), time_(0)
{
}

void SwordBossMove::Update()
{
	time_--;
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetMoveAction()->SetTarget(GameManager::GetPlayer()->GetPosition());

	if (e->GetMoveAction()->IsInRange() || (time_ % 5 == 0 && e->GetMoveAction()->IsOutTarget(3.0f)) ) {
		e->GetMoveAction()->UpdatePath(GameManager::GetPlayer()->GetPosition());
	}

	e->GetMoveAction()->Update();
	e->GetRotateAction()->Update();

	//移動時間終了
	if(time_ <= 0) owner_->ChangeState("Wait");
}

void SwordBossMove::OnEnter()
{
	time_ = FPS * MIN_MOVE_TIME + rand() % MAX_MOVE_TIME * FPS;
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetMoveAction()->SetMoveSpeed(FAST_SPEED);
	e->GetAnimationController()->SetNextAnime((int)SWORDBOSS_ANIMATION::RUN, 0.1f);

}

void SwordBossMove::OnExit()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetMoveAction()->StopMove();
}

//-------------------------------------Attack-------------------------------------------

SwordBossAttack::SwordBossAttack(StateManager* owner) : StateBase(owner), time_(0)
{
}

void SwordBossAttack::Update()
{
	time_++;
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());

	//回転やら移動やら
	if (time_ < ROTATE_FRAME) {
		e->GetRotateAction()->Update();
		e->GetOrientedMoveAction()->SetTarget(GameManager::GetPlayer()->GetPosition());
		e->GetOrientedMoveAction()->Update();
	}

	int startT = e->GetAnimationController()->GetAnim((int)SWORDBOSS_ANIMATION::ATTACK1).startFrame;
	int endT = e->GetAnimationController()->GetAnim((int)SWORDBOSS_ANIMATION::ATTACK1).endFrame;
	int time1 = (endT - startT);
	
	//if (time_ == time1) e->GetAnimationController()->SetNextAnime((int)SWORDBOSS_ANIMATION::RUN_ATTACK, 0.1f);

	//int startT2 = e->GetAnimationController()->GetAnim((int)SWORDBOSS_ANIMATION::RUN_ATTACK).startFrame;
	//int endT2 = e->GetAnimationController()->GetAnim((int)SWORDBOSS_ANIMATION::RUN_ATTACK).endFrame;
	//int time2 = time1 + (endT2 - startT2);

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

	if (time_ >= time1) {
		owner_->ChangeState("Wait");
	}
}

void SwordBossAttack::OnEnter()
{
	time_ = 0;
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetAnimationController()->SetNextAnime((int)SWORDBOSS_ANIMATION::RUN, 0.1f);
	e->GetOrientedMoveAction()->SetDirection(XMVECTOR{ 0, 0, 1, 0 });
	e->SetCombatReady(false);
}

void SwordBossAttack::OnExit()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetRotateAction()->SetRatio(ROTATE_RATIO);
	e->SetAttackCoolDown(rand() % 100);
	e->DamageInfoReset();
}

//--------------------------------------------------------------------------------
#include "StoneGolemState.h"
#include "StateManager.h"
#include "../Player/Player.h"
#include "../Player/Aim.h"
#include "../Enemy/StoneGolem.h"
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

	static const float FAST_SPEED = 0.03f;
	static const float SLOW_SPEED = 0.02f;
	static const float ROTATE_RATIO = 0.07f;

	//攻撃Stateの情報
	static const int ATTACK_FRAME[2] = { 0, 300 };
	static const float ATTACK_ROTATE_RATIO = 0.05f;
	static const int CALC_FRAME1[2] = { 65, 85 };
	static const int CALC_FRAME2[2] = { 120, 140 };
	static const int CALC_FRAME3[2] = { 235, 247 };
	static const float COMBO_ATTACK_DIST = 3.0f;

	//攻撃１の情報
	static const int ROTATE_FRAME = 30;
	//攻撃３の情報
	static const int ROTATE_FRAME3[2] = { 140, 230 };
	static const int ATTACK_EFFECT_TIME[2] = { 244, 247 };
	static const float MOVESPEED_FRAME3 = 0.02f;

}

StoneGolemAppear::StoneGolemAppear(StateManager* owner) : StateBase(owner), time_(0)
{
}

void StoneGolemAppear::Update()
{
	time_++;
	StoneGolem* e = static_cast<StoneGolem*>(owner_->GetGameObject());
	
	float tsize = (float)time_ / (float)APPER_TIME * 0.8f;
	e->SetScale(XMFLOAT3(tsize, tsize, tsize));
	
	if (time_ > APPER_TIME) owner_->ChangeState("Patrol");
}

void StoneGolemAppear::OnEnter()
{
	StoneGolem* e = static_cast<StoneGolem*>(owner_->GetGameObject());
	XMFLOAT3 pos = e->GetPosition();
	VFXManager::CreatVfxEnemySpawn(pos, APPER_TIME);

}

void StoneGolemAppear::OnExit()
{
	StoneGolem* e = static_cast<StoneGolem*>(owner_->GetGameObject());
	float size = 0.8f;
	e->SetScale(XMFLOAT3(size, size, size));

}

//--------------------------------------------------------------------------------

StoneGolemDead::StoneGolemDead(StateManager* owner) : StateBase(owner), time_(0)
{
}

void StoneGolemDead::Update()
{
	time_++;
	StoneGolem* e = static_cast<StoneGolem*>(owner_->GetGameObject());
	
	float s = (float)time_ / (float)DEAD_TIME;
	s = (1.0f - s) * 0.8f;
	e->SetScale({ s, s, s });

	if (time_ >= DEAD_TIME) e->DeadExit();
}

void StoneGolemDead::OnEnter()
{
	StoneGolem* e = static_cast<StoneGolem*>(owner_->GetGameObject());
	e->DeadEnter();
	time_ = 0;
}

//--------------------------------------------------------------------------------

StoneGolemPatrol::StoneGolemPatrol(StateManager* owner) : StateBase(owner), foundSearchTime_(0)
{
}

void StoneGolemPatrol::Update()
{
	//Astar移動が終わったなら更新・待ち時間適当にrandamで デバッグ用
	StoneGolem* e = static_cast<StoneGolem*>(owner_->GetGameObject());
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

void StoneGolemPatrol::OnEnter()
{
	StoneGolem* e = static_cast<StoneGolem*>(owner_->GetGameObject());
	e->GetMoveAction()->SetMoveSpeed(SLOW_SPEED);
	e->GetRotateAction()->SetTarget(nullptr);
	e->GetRotateAction()->SetRatio(ROTATE_RATIO);

}

void StoneGolemPatrol::OnExit()
{
	StoneGolem* e = static_cast<StoneGolem*>(owner_->GetGameObject());
	e->GetMoveAction()->StopMove();
}

//--------------------------------------------------------------------------------

StoneGolemCombat::StoneGolemCombat(StateManager* owner) : StateBase(owner), time_(0)
{
	StoneGolem* e = static_cast<StoneGolem*>(owner_->GetGameObject());
	
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

void StoneGolemCombat::Update()
{
	time_++;
	if (time_ % 10 == 0) root_->Update();
	
	StoneGolem* e = static_cast<StoneGolem*>(owner_->GetGameObject());
	e->GetCombatStateManager()->Update();

}

void StoneGolemCombat::OnEnter()
{
	StoneGolem* e = static_cast<StoneGolem*>(owner_->GetGameObject());
	e->GetEnemyUi()->InitTargetFoundUi();
	e->GetRotateAction()->Initialize();
	e->GetRotateAction()->SetTarget(GameManager::GetPlayer());
	e->GetRotateAction()->SetRatio(ROTATE_RATIO);

}

StoneGolemCombat::~StoneGolemCombat()
{
	delete root_;
}

//-------------------------------------CombatState-------------------------------------------

StoneGolemWait::StoneGolemWait(StateManager* owner) : StateBase(owner), time_(0)
{
}

void StoneGolemWait::Update()
{
	time_++;
	StoneGolem* e = static_cast<StoneGolem*>(owner_->GetGameObject());
	e->GetRotateAction()->Update();

	//壁に当たったか調べて
	e->GetOrientedMoveAction()->SetTarget(GameManager::GetPlayer()->GetPosition());
	if (e->GetOrientedMoveAction()->CheckWallCollision(2)) {
		e->GetOrientedMoveAction()->InverseDirection();
	}

	XMFLOAT3 pos = e->GetPosition();
	XMFLOAT3 pPos = GameManager::GetPlayer()->GetPosition();
	pos = { pos.x - pPos.x, 0.0f, pos.z - pPos.z };
	float dist = sqrtf(pos.x * pos.x + pos.z * pos.z);
	if (time_ > (FPS * 3) && dist < e->GetCombatDistance()) {
		e->GetOrientedMoveAction()->SelectProbabilityDirection(0, 0, 1, 1);
		time_ = 0;
	}

	//時間とランダムで向き計算
	if (time_ > (FPS * 3) && rand() % 10 == 0) {
		e->GetOrientedMoveAction()->CalcOptimalDirection();
		time_ = 0;
	}

	e->GetOrientedMoveAction()->Update();

}

void StoneGolemWait::OnEnter()
{
	time_ = 0;
	StoneGolem* e = static_cast<StoneGolem*>(owner_->GetGameObject());
	e->SetActionCoolDown((FPS * MIN_MOVE_TIME) + (rand() % MAX_MOVE_TIME) * FPS);
	e->GetOrientedMoveAction()->SetMoveSpeed(SLOW_SPEED);
	
	//プレイヤーから指定の範囲内で
	//ゲーム参考にして
	XMFLOAT3 pPos = GameManager::GetPlayer()->GetPosition();
	XMFLOAT3 ePos = e->GetPosition();
	XMFLOAT3 vec = { pPos.x - ePos.x, 0.0f, pPos.z - ePos.z };
	float dist = sqrt(vec.x * vec.x + vec.z * vec.z);
	if (dist <= e->GetCombatDistance()) e->GetOrientedMoveAction()->SetDirection(XMVECTOR{ 0, 0, 1, 0 });
	else {
		if (rand() % 2 == 0) e->GetOrientedMoveAction()->SetDirection(XMVECTOR{ 1, 0, 0, 0 });
		else e->GetOrientedMoveAction()->SetDirection(XMVECTOR{ -1, 0, 0, 0 });
	}
}

//------------------------------------Move--------------------------------------------

StoneGolemMove::StoneGolemMove(StateManager* owner) : StateBase(owner), time_(0)
{
}

void StoneGolemMove::Update()
{
	time_--;
	StoneGolem* e = static_cast<StoneGolem*>(owner_->GetGameObject());
	e->GetMoveAction()->SetTarget(GameManager::GetPlayer()->GetPosition());

	XMFLOAT3 pos = e->GetPosition();
	XMFLOAT3 pPos = GameManager::GetPlayer()->GetPosition();
	pos = { pos.x - pPos.x, 0.0f, pos.z - pPos.z };
	float dist = sqrtf(pos.x * pos.x + pos.z * pos.z);
	if (e->GetMoveAction()->IsInRange() && dist > e->GetAttackDistance()) {
		e->GetMoveAction()->UpdatePath(GameManager::GetPlayer()->GetPosition());
	}
		
	if((time_ % 5 == 0 && e->GetMoveAction()->IsOutTarget(3.0f)) ) {
		e->GetMoveAction()->UpdatePath(GameManager::GetPlayer()->GetPosition());
	}

	e->GetMoveAction()->Update();
	e->GetRotateAction()->Update();

	//移動時間終了
	if(time_ <= 0) owner_->ChangeState("Wait");
}

void StoneGolemMove::OnEnter()
{
	StoneGolem* e = static_cast<StoneGolem*>(owner_->GetGameObject());
	e->GetMoveAction()->SetMoveSpeed(FAST_SPEED);
	time_ = FPS * MIN_MOVE_TIME + rand() % MAX_MOVE_TIME * FPS;
}

void StoneGolemMove::OnExit()
{
	StoneGolem* e = static_cast<StoneGolem*>(owner_->GetGameObject());
	e->GetMoveAction()->StopMove();
}

//-------------------------------------Attack-------------------------------------------

StoneGolemAttack::StoneGolemAttack(StateManager* owner) : StateBase(owner), time_(0)
{
}

void StoneGolemAttack::Update()
{
	time_++;
	StoneGolem* e = static_cast<StoneGolem*>(owner_->GetGameObject());

	//攻撃を続けるか計算
	if (time_ == 100 || time_ == 150) {
		XMFLOAT3 pPos = GameManager::GetPlayer()->GetPosition();
		XMFLOAT3 ePos = e->GetPosition();
		XMFLOAT3 vec = { pPos.x - ePos.x, 0.0f, pPos.z - ePos.z };
		float dist = sqrt(vec.x * vec.x + vec.z * vec.z);
		if (dist > COMBO_ATTACK_DIST) owner_->ChangeState("Wait");
	}

	//回転やら移動やら
	if (time_ < ROTATE_FRAME) e->GetRotateAction()->Update();
	else if (time_ >= ROTATE_FRAME3[0] && time_ <= ROTATE_FRAME3[1]) {
		e->GetRotateAction()->Update();
		e->GetOrientedMoveAction()->SetTarget(GameManager::GetPlayer()->GetPosition());
		e->GetOrientedMoveAction()->Update();
	}

	//攻撃フラグの制御
	if (time_ == CALC_FRAME1[0]) { e->SetDamageInfoCombo1(); }
	else if (time_ == CALC_FRAME1[1]) { e->DamageInfoReset(); }
	
	else if (time_ == CALC_FRAME2[0]) { e->SetDamageInfoCombo2(); }
	else if (time_ == CALC_FRAME2[1]) { e->DamageInfoReset(); }
	
	else if (time_ == CALC_FRAME3[0]) { e->SetDamageInfoCombo3(); }
	else if (time_ == CALC_FRAME3[1]) { e->DamageInfoReset(); }

	//エフェクト
	if (time_ >= ATTACK_EFFECT_TIME[0] && time_ <= ATTACK_EFFECT_TIME[1]) {
		XMFLOAT3 pos = e->GetPosition();
		XMFLOAT3 cP = e->GetAttackColliderList().front()->GetCenter();
		pos = { pos.x + cP.x, 0.0f , pos.z + cP.z };
		VFXManager::CreatVfxSmoke(pos);

		//カメラシェイク
		if (time_ == ATTACK_EFFECT_TIME[0]) {
			const float maxRange = 8.0f;
			XMFLOAT3 pPos = GameManager::GetPlayer()->GetPosition();
			pos = { pPos.x - pos.x, 0.0f, pPos.z - pos.z };
			float range = sqrt(pos.x * pos.x + pos.z * pos.z);
			if (range <= maxRange) {
				range = (1.0f - (range / maxRange));
				GameManager::GetPlayer()->GetAim()->SetCameraShakeDirection(XMVECTOR{ 0.0f, 1.0f, 0.0f, 0.0f });
				GameManager::GetPlayer()->GetAim()->SetCameraShake(7, 0.25f * range, 0.7f, 0.3f, 0.8f);
			}
		}
	}

	if (time_ >= ATTACK_FRAME[1]) {
		owner_->ChangeState("Wait");
	}
}

void StoneGolemAttack::OnEnter()
{
	time_ = 0;
	StoneGolem* e = static_cast<StoneGolem*>(owner_->GetGameObject());
	Model::SetAnimFrame(e->GetModelHandle(), ATTACK_FRAME[0], ATTACK_FRAME[1], 1.0f);
	e->GetOrientedMoveAction()->SetDirection(XMVECTOR{ 0, 0, 1, 0 });
	e->GetOrientedMoveAction()->SetMoveSpeed(MOVESPEED_FRAME3);
	e->GetRotateAction()->SetRatio(ATTACK_ROTATE_RATIO);
	e->SetCombatReady(false);
}

void StoneGolemAttack::OnExit()
{
	StoneGolem* e = static_cast<StoneGolem*>(owner_->GetGameObject());
	Model::SetAnimFrame(e->GetModelHandle(), 0, 0, 1.0f);
	e->GetOrientedMoveAction()->SetMoveSpeed(MOVESPEED_FRAME3);
	e->GetRotateAction()->SetRatio(ROTATE_RATIO);
	e->SetAttackCoolDown(rand() % 100);
	e->DamageInfoReset();

}

//--------------------------------------------------------------------------------
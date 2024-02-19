#include "StoneGolemState.h"
#include "StateManager.h"
#include "../Player/Player.h"
#include "../Player/Aim.h"
#include "../Enemy/StoneGolem.h"
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
	static const float FAST_SPEED = 0.05f;
	static const float SLOW_SPEED = 0.04f;
	static const float ROTATE_RATIO = 0.07f;

	//攻撃Stateの情報
	static const int ATTACK_FRAME[2] = { 0, 300 };
	static const float ATTACK_ROTATE_RATIO = 0.05f;
	static const int CALC_FRAME1[2] = { 65, 85 };
	static const int CALC_FRAME2[2] = { 120, 140 };
	static const int CALC_FRAME3[2] = { 235, 247 };
	//攻撃１の情報
	static const int ROTATE_FRAME = 30;
	//攻撃３の情報
	static const int ROTATE_FRAME3[2] = { 140, 230 };
	static const int ATTACK_EFFECT_TIME[2] = { 244, 247 };
	static const float MOVESPEED_FRAME3 = 0.03f;

}

StoneGolemAppear::StoneGolemAppear(StateManager* owner) : StateBase(owner), time_(0)
{
}

void StoneGolemAppear::Update()
{
	time_++;
	if (time_ > APPER_TIME) owner_->ChangeState("Patrol");

	float tsize = (float)time_ / (float)APPER_TIME * 1.2f;
	StoneGolem* f = static_cast<StoneGolem*>(owner_->GetGameObject());
	f->SetScale(XMFLOAT3(tsize, tsize, tsize));

}

void StoneGolemAppear::OnEnter()
{
	StoneGolem* f = static_cast<StoneGolem*>(owner_->GetGameObject());
	XMFLOAT3 pos = f->GetPosition();
	VFXManager::CreatVfxEnemySpawn(pos, APPER_TIME);

}

void StoneGolemAppear::OnExit()
{
	StoneGolem* f = static_cast<StoneGolem*>(owner_->GetGameObject());
	float size = 1.2f;
	f->SetScale(XMFLOAT3(size, size, size));

}

//--------------------------------------------------------------------------------

StoneGolemPatrol::StoneGolemPatrol(StateManager* owner) : StateBase(owner), foundSearchTime_(0)
{
}

void StoneGolemPatrol::Update()
{
	//Astar移動が終わったなら更新・待ち時間適当にrandamで デバッグ用
	StoneGolem* f = static_cast<StoneGolem*>(owner_->GetGameObject());
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

void StoneGolemPatrol::OnEnter()
{
	StoneGolem* f = static_cast<StoneGolem*>(owner_->GetGameObject());
	f->GetMoveAction()->SetMoveSpeed(SLOW_SPEED);
	f->GetRotateAction()->SetTarget(nullptr);
	f->GetRotateAction()->SetRatio(ROTATE_RATIO);

}

void StoneGolemPatrol::OnExit()
{
	StoneGolem* f = static_cast<StoneGolem*>(owner_->GetGameObject());
	f->GetMoveAction()->SetMoveSpeed(FAST_SPEED);
	f->GetMoveAction()->StopMove();
}

//--------------------------------------------------------------------------------

StoneGolemCombat::StoneGolemCombat(StateManager* owner) : StateBase(owner), time_(0)
{
	StoneGolem* f = static_cast<StoneGolem*>(owner_->GetGameObject());
	
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

void StoneGolemCombat::Update()
{
	time_++;
	if (time_ % 10 == 0) root_->Update();
	
	StoneGolem* f = static_cast<StoneGolem*>(owner_->GetGameObject());
	f->GetCombatStateManager()->Update();

}

void StoneGolemCombat::OnEnter()
{
	StoneGolem* f = static_cast<StoneGolem*>(owner_->GetGameObject());
	f->GetEnemyUi()->InitTargetFoundUi();
	f->GetRotateAction()->Initialize();
	f->GetRotateAction()->SetTarget(GameManager::GetPlayer());
	f->GetRotateAction()->SetRatio(ROTATE_RATIO);

}

StoneGolemCombat::~StoneGolemCombat()
{
	delete root_;
}

//--------------------------------------------------------------------------------

StoneGolemDead::StoneGolemDead(StateManager* owner) : StateBase(owner)
{
}

void StoneGolemDead::Update()
{
}

//-------------------------------------CombatState-------------------------------------------

StoneGolemWait::StoneGolemWait(StateManager* owner) : StateBase(owner)
{
}

void StoneGolemWait::Update()
{
	StoneGolem* f = static_cast<StoneGolem*>(owner_->GetGameObject());
	f->GetRotateAction()->Update();
	f->GetOrientedMoveAction()->SetTarget(GameManager::GetPlayer()->GetPosition());
	f->GetOrientedMoveAction()->Update();

}

void StoneGolemWait::OnEnter()
{
	StoneGolem* f = static_cast<StoneGolem*>(owner_->GetGameObject());
	f->GetMoveAction()->SetMoveSpeed(SLOW_SPEED);
	f->GetOrientedMoveAction()->SetDirection(XMVECTOR{ 0, 0, -1, 0 });

}

//--------------------------------------------------------------------------------

StoneGolemMove::StoneGolemMove(StateManager* owner) : StateBase(owner)
{
}

void StoneGolemMove::Update()
{
	//らんｄやめよう
	StoneGolem* f = static_cast<StoneGolem*>(owner_->GetGameObject());
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

void StoneGolemMove::OnEnter()
{
	StoneGolem* f = static_cast<StoneGolem*>(owner_->GetGameObject());
	f->GetMoveAction()->SetMoveSpeed(FAST_SPEED);

}

void StoneGolemMove::OnExit()
{
	StoneGolem* f = static_cast<StoneGolem*>(owner_->GetGameObject());
	f->GetMoveAction()->StopMove();
}

//--------------------------------------------------------------------------------

StoneGolemAttack::StoneGolemAttack(StateManager* owner) : StateBase(owner), time_(0)
{
}

void StoneGolemAttack::Update()
{
	time_++;
	StoneGolem* f = static_cast<StoneGolem*>(owner_->GetGameObject());
	
	//回転やら移動やら
	if (time_ < ROTATE_FRAME) f->GetRotateAction()->Update();
	else if (time_ >= ROTATE_FRAME3[0] && time_ <= ROTATE_FRAME3[1]) {
		f->GetRotateAction()->Update();
		f->GetOrientedMoveAction()->SetTarget(GameManager::GetPlayer()->GetPosition());
		f->GetOrientedMoveAction()->Update();
	}

	//攻撃フラグの制御
	//StoneGolemのOnAttackCollisionの方でPlayerに当たったらfalseにする処理を書いてある
	if (time_ == CALC_FRAME1[0]) { f->GetSphereCollider(0)->SetValid(true); f->GetSphereCollider(1)->SetValid(true); }
	else if (time_ == CALC_FRAME1[1]) { f->GetSphereCollider(0)->SetValid(false); f->GetSphereCollider(1)->SetValid(false); }
	else if (time_ == CALC_FRAME2[0]) { f->GetSphereCollider(0)->SetValid(true); f->GetSphereCollider(1)->SetValid(true); }
	else if (time_ == CALC_FRAME2[1]) { f->GetSphereCollider(0)->SetValid(false); f->GetSphereCollider(1)->SetValid(false); }
	else if (time_ == CALC_FRAME3[0]) { f->GetSphereCollider(0)->SetValid(true); f->GetSphereCollider(1)->SetValid(true); }
	else if (time_ == CALC_FRAME3[1]) { f->GetSphereCollider(0)->SetValid(false); f->GetSphereCollider(1)->SetValid(false); }

	//エフェクト
	if (time_ >= ATTACK_EFFECT_TIME[0] && time_ <= ATTACK_EFFECT_TIME[1]) {
		XMFLOAT3 pos = f->GetPosition();
		XMFLOAT3 cP = f->GetSphereCollider(0)->GetCenter();
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
				GameManager::GetPlayer()->GetAim()->SetCameraShake(7, 0.3f * range, 0.7f, 0.3f, 0.8f);
			}
		}
	}

	if (time_ >= ATTACK_FRAME[1]) {
		Model::SetAnimFrame(f->GetModelHandle(), 0, 0, 1.0f);
		owner_->ChangeState("Wait");
		return;
	}
}

void StoneGolemAttack::OnEnter()
{
	time_ = 0;
	StoneGolem* f = static_cast<StoneGolem*>(owner_->GetGameObject());
	Model::SetAnimFrame(f->GetModelHandle(), ATTACK_FRAME[0], ATTACK_FRAME[1], 1.0f);
	f->GetOrientedMoveAction()->SetDirection(XMVECTOR{ 0, 0, 1, 0 });
	f->GetOrientedMoveAction()->SetMoveSpeed(MOVESPEED_FRAME3);
	f->GetRotateAction()->SetRatio(ATTACK_ROTATE_RATIO);
}

void StoneGolemAttack::OnExit()
{
	StoneGolem* f = static_cast<StoneGolem*>(owner_->GetGameObject());
	f->GetOrientedMoveAction()->SetMoveSpeed(MOVESPEED_FRAME3);
	f->GetRotateAction()->SetRatio(ROTATE_RATIO);

}

//--------------------------------------------------------------------------------

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
#include "../BehaviorTree/IsEnemyStateNode.h"
#include "../BehaviorTree/IsEnemyAttackPermission.h"

#include "../Action/MoveAction.h"
#include "../Action/RotateAction.h"
#include "../Action/SearchAction.h"

namespace {
	static const int FOUND_SEARCH = 10;		//視覚探知の更新時間
	static const int APPER_TIME = 180;
	static const float FAST_SPEED = 0.05f;
	static const float SLOW_SPEED = 0.03f;
	static const float ROTATE_RATIO = 0.07f;

	static const int FPS = 60;
	static const int MAX_MOVE_TIME = 5;

	//攻撃Stateの情報
	static const int ATTACK_FRAME[2] = { 0, 300 };
	static const float ATTACK_ROTATE_RATIO = 0.05f;
	static const int CALC_FRAME1[2] = { 65, 85 };
	static const int CALC_FRAME2[2] = { 120, 140 };
	static const int CALC_FRAME3[2] = { 235, 247 };
	//攻撃１の情報
	static const int ROTATE_FRAME = 30;
	static const float ATTACK_READY_DISTANCE = 2.0f;
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

	float tsize = (float)time_ / (float)APPER_TIME;
	StoneGolem* e = static_cast<StoneGolem*>(owner_->GetGameObject());
	e->SetScale(XMFLOAT3(tsize, tsize, tsize));

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
	e->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));

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
	e->GetMoveAction()->SetMoveSpeed(FAST_SPEED);
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
	Selector* attackSelector = new Selector();
	IsEnemyCombatState* wCon = new IsEnemyCombatState(waitSelector, "Wait", e);
	IsEnemyCombatState* mCon = new IsEnemyCombatState(moveSelector, "Move", e);
	selector1->AddChildren(wCon);
	selector1->AddChildren(mCon);

	//-------------------------------------Wait--------------------------------------
	EnemyChangeCombatStateNode* action1 = new EnemyChangeCombatStateNode(e, "Move");
	IsEnemyAttackPermission* condition2 = new IsEnemyAttackPermission(action1, e);
	IsEnemyAttackReady* condition3 = new IsEnemyAttackReady(condition2, e);
	waitSelector->AddChildren(condition3);

	//-------------------------------------Move--------------------------------------
	EnemyChangeCombatStateNode* action2 = new EnemyChangeCombatStateNode(e, "Attack");
	IsPlayerInRangeNode* condition4 = new IsPlayerInRangeNode(action2, ATTACK_READY_DISTANCE, e, GameManager::GetPlayer());
	moveSelector->AddChildren(condition4);

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

StoneGolemWait::StoneGolemWait(StateManager* owner) : StateBase(owner)
{
}

void StoneGolemWait::Update()
{
	StoneGolem* e = static_cast<StoneGolem*>(owner_->GetGameObject());
	e->GetRotateAction()->Update();

	//壁に当たったか調べて
	e->GetOrientedMoveAction()->SetTarget(GameManager::GetPlayer()->GetPosition());
	if (e->GetOrientedMoveAction()->CheckWallCollision(1)) {
		e->GetOrientedMoveAction()->SetDirection(XMVector3Normalize(XMVECTOR{ 0.0f, 0.0f, 1.0f, 0.0f }));
	}
	
	e->GetOrientedMoveAction()->Update();

}

void StoneGolemWait::OnEnter()
{
	StoneGolem* e = static_cast<StoneGolem*>(owner_->GetGameObject());
	e->GetMoveAction()->SetMoveSpeed(SLOW_SPEED);

	//プレイヤーから指定の範囲内で
	//ゲーム参考にしてから作る
	XMFLOAT3 pPos = GameManager::GetPlayer()->GetPosition();
	XMFLOAT3 ePos = e->GetPosition();
	XMFLOAT3 vec = { pPos.x - ePos.x, 0.0f, pPos.z - ePos.z };
	float dist = sqrt(vec.x * vec.x + vec.z * vec.z);
	if(dist <= e->GetCombatDistance()) e->GetOrientedMoveAction()->SetDirection(XMVECTOR{ 0, 0, 1, 0 });
	else {
		int r = rand() % 5;
		if(r == 0 || r == 1) e->GetOrientedMoveAction()->SetDirection(XMVECTOR{ 1, 0, 0, 0 });
		else if (r == 2 || r == 3) e->GetOrientedMoveAction()->SetDirection(XMVECTOR{ -1, 0, 0, 0 });
		else e->GetOrientedMoveAction()->SetDirection(XMVECTOR{ 0, 0, -1, 0 });
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

	if (e->GetMoveAction()->IsInRange() || (time_ % 5 == 0 && e->GetMoveAction()->IsOutTarget(3.0f)) ) {
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
	time_ = rand() % MAX_MOVE_TIME * FPS;
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
	
	//回転やら移動やら
	if (time_ < ROTATE_FRAME) e->GetRotateAction()->Update();
	else if (time_ >= ROTATE_FRAME3[0] && time_ <= ROTATE_FRAME3[1]) {
		e->GetRotateAction()->Update();
		e->GetOrientedMoveAction()->SetTarget(GameManager::GetPlayer()->GetPosition());
		e->GetOrientedMoveAction()->Update();
	}

	//攻撃フラグの制御
	//StoneGolemのOnAttackCollisionの方でPlayerに当たったらfalseにする処理を書いてある
	if (time_ == CALC_FRAME1[0]) { e->GetSphereCollider(0)->SetValid(true); e->GetSphereCollider(1)->SetValid(true); }
	else if (time_ == CALC_FRAME1[1]) { e->GetSphereCollider(0)->SetValid(false); e->GetSphereCollider(1)->SetValid(false); }
	else if (time_ == CALC_FRAME2[0]) { e->GetSphereCollider(0)->SetValid(true); e->GetSphereCollider(1)->SetValid(true); }
	else if (time_ == CALC_FRAME2[1]) { e->GetSphereCollider(0)->SetValid(false); e->GetSphereCollider(1)->SetValid(false); }
	else if (time_ == CALC_FRAME3[0]) { e->GetSphereCollider(0)->SetValid(true); e->GetSphereCollider(1)->SetValid(true); }
	else if (time_ == CALC_FRAME3[1]) { e->GetSphereCollider(0)->SetValid(false); e->GetSphereCollider(1)->SetValid(false); }

	//エフェクト
	if (time_ >= ATTACK_EFFECT_TIME[0] && time_ <= ATTACK_EFFECT_TIME[1]) {
		XMFLOAT3 pos = e->GetPosition();
		XMFLOAT3 cP = e->GetSphereCollider(0)->GetCenter();
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
		Model::SetAnimFrame(e->GetModelHandle(), 0, 0, 1.0f);
		owner_->ChangeState("Wait");
		return;
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
}

void StoneGolemAttack::OnExit()
{
	StoneGolem* e = static_cast<StoneGolem*>(owner_->GetGameObject());
	e->GetOrientedMoveAction()->SetMoveSpeed(MOVESPEED_FRAME3);
	e->GetRotateAction()->SetRatio(ROTATE_RATIO);

	e->SetAttackCoolDown(rand() % 100);
}

//--------------------------------------------------------------------------------

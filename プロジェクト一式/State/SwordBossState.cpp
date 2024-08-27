#include "SwordBossState.h"
#include "StateManager.h"
#include "../Other/VFXManager.h"
#include "../Animation/AnimationController.h"
#include "../GameManager/GameManager.h"
#include "../Player/Player.h"
#include "../Player/Aim.h"
#include "../Enemy/SwordBoss.h"
#include "../Enemy/EnemyUi.h"
#include "../Enemy/EnemyAttackSelect.h"
#include "../Stage/CreateStage.h"
#include "../Engine/Model.h"
#include "../Engine/Global.h"
#include "../Engine/Easing.h"

#include "../BehaviorTree/EnemySetValueNode.h"
#include "../BehaviorTree/IsEnemyActionReadyNode.h"
#include "../BehaviorTree/ChangeStateNode.h"
#include "../BehaviorTree/BehaviourNode.h"
#include "../BehaviorTree/PlayerConditionNode.h"
#include "../BehaviorTree/IsEnemyStateNode.h"
#include "../BehaviorTree/IsEnemyPermission.h"
#include "../BehaviorTree/EnemyAttackSelectNode.h"

#include "../Action/MoveAction.h"
#include "../Action/RotateAction.h"

namespace {
	static const int APPER_TIME = 180;
	static const int FOUND_SEARCH = 10;

	static const int MIN_MOVE_TIME = 60;
	static const int RANDOM_MOVE_TIME = 120;

	static const float FAST_SPEED = 0.055f;
	static const float SLOW_SPEED = 0.03f;
	static const float ROTATE_RATIO = 0.07f;

	//攻撃---------------------------------------------------
	static const int SELECT_COOLDOWN = 5;	//SelectAttackの計算間隔（選べなかった時用に）

	//Thrust
	static const float ThrustMoveSpeed = 0.22f;
	static const int ThrustMoveTime[2] = { 40, 60 };

	//Slash_Jump
	static const float JumpMoveSpeed = 0.24f;
	static const float JumpheightPower = 0.07f;
	static const int JumpMoveTime[2] = { 43, 60 };

}

void SwordBossAppear::Update()
{
	time_++;
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	
	float tsize = (float)time_ / (float)APPER_TIME;
	e->SetScale(XMFLOAT3(tsize, tsize, tsize));

	if (time_ > APPER_TIME) owner_->ChangeState("Combat");
}

void SwordBossAppear::OnEnter()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetAnimationController()->SetNextAnim((int)SWORDBOSS_ANIMATION::WALK, 0.1f);

	XMFLOAT3 pos = e->GetPosition();
	VFXManager::CreateVfxEnemySpawn(pos);

	OutputDebugString("Apper\n");

}

void SwordBossAppear::OnExit()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
}

//--------------------------------------Dead------------------------------------------

void SwordBossDead::Update()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	time_++;

	int startT = e->GetAnimationController()->GetAnim((int)SWORDBOSS_ANIMATION::DEAD).startFrame;
	int endT = e->GetAnimationController()->GetAnim((int)SWORDBOSS_ANIMATION::DEAD).endFrame;
	int allTime = (endT - startT);
	
	if (time_ >= allTime) {
		Model::SetAnimStop(e->GetModelHandle(), true);
		e->DeadExit();
	}
}

void SwordBossDead::OnEnter()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetAnimationController()->SetNextAnim((int)SWORDBOSS_ANIMATION::DEAD, 0.1f);
	e->DeadEnter();
	e->AttackEnd();
	time_ = 0;
	
	OutputDebugString("Dead\n");

}

//--------------------------------------Combat------------------------------------------

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
	
	//攻撃準備可能なら攻撃どれか選択して、選択できたならState推移
	EnemyChangeCombatStateNode* action3 = new EnemyChangeCombatStateNode(e, "Attack");
	SwordBossAttackSelectNode* action2 = new SwordBossAttackSelectNode(e);
	EnemySetAttackCoolDown* action4 = new EnemySetAttackCoolDown(e, SELECT_COOLDOWN);
	Sequence* sequence1 = new Sequence();
	sequence1->AddChildren(action2);
	sequence1->AddChildren(action3);
	sequence1->AddChildren(action4);

	IsEnemyAttackPermission* condition5 = new IsEnemyAttackPermission(sequence1, e);
	IsEnemyAttackReady* condition8 = new IsEnemyAttackReady(condition5, e);
	waitSelector->AddChildren(condition8);

	EnemyChangeCombatStateNode* action1 = new EnemyChangeCombatStateNode(e, "Move");
	IsEnemyMovePermission* condition2 = new IsEnemyMovePermission(action1, e);
	IsEnemyActionReady* condition3 = new IsEnemyActionReady(condition2, e);
	//waitSelector->AddChildren(condition3);

	//-------------------------------------Move--------------------------------------
	moveSelector->AddChildren(condition8);

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
	e->GetCombatStateManager()->ChangeState("Wait");

	OutputDebugString("Combat\n");
}

SwordBossCombat::~SwordBossCombat()
{
	delete root_;
}

//-------------------------------------CombatState-------------------------------------------

void SwordBossWait::Update()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetRotateAction()->Update();

	//壁に当たったか調べて
	e->GetOrientedMoveAction()->SetTarget(GameManager::GetPlayer()->GetPosition());
	if (e->GetOrientedMoveAction()->CheckWallCollision(3)) {
		e->GetOrientedMoveAction()->SetDirection(XMVector3Normalize(XMVECTOR{ 0.0f, 0.0f, 1.0f, 0.0f }));
	}
	
	e->GetOrientedMoveAction()->Update();

}

void SwordBossWait::OnEnter()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetAstarMoveAction()->SetMoveSpeed(SLOW_SPEED);
	e->GetOrientedMoveAction()->SetMoveSpeed(SLOW_SPEED);
	e->GetAnimationController()->SetNextAnim((int)SWORDBOSS_ANIMATION::WALK, 0.1f);

	//プレイヤーから指定の範囲内で
	XMFLOAT3 pPos = GameManager::GetPlayer()->GetPosition();
	XMFLOAT3 ePos = e->GetPosition();
	XMFLOAT3 vec = { pPos.x - ePos.x, 0.0f, pPos.z - ePos.z };
	float dist = sqrt(vec.x * vec.x + vec.z * vec.z);
	
	//遠いなら前、近いなら左右どっちか
	if(dist >= e->GetCombatDistance()) e->GetOrientedMoveAction()->SetDirection(XMVECTOR{ 0, 0, 1, 0 });
	else e->GetOrientedMoveAction()->SelectProbabilityDirection(0, 0, 1, 1);

	OutputDebugString("Wait\n");

}

//------------------------------------Move--------------------------------------------

void SwordBossMove::Update()
{
	time_--;
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetAstarMoveAction()->SetTarget(GameManager::GetPlayer()->GetPosition());

	float plaDist = CalculationDistance(e->GetPosition(), GameManager::GetPlayer()->GetPosition());
	if (plaDist <= 5.0f) {
		owner_->ChangeState("Wait");
		return;
	}

	if (e->GetAstarMoveAction()->IsInRange() || (time_ % 5 == 0 && e->GetAstarMoveAction()->IsOutTarget(3.0f)) ) {
		e->GetAstarMoveAction()->UpdatePath(GameManager::GetPlayer()->GetPosition());
	}

	e->GetAstarMoveAction()->Update();
	e->GetRotateAction()->Update();

	//移動時間終了
	if(time_ <= 0) owner_->ChangeState("Wait");
}

void SwordBossMove::OnEnter()
{
	time_ = MIN_MOVE_TIME + rand() % RANDOM_MOVE_TIME;
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetAstarMoveAction()->SetMoveSpeed(FAST_SPEED);
	e->GetOrientedMoveAction()->SetMoveSpeed(FAST_SPEED);
	e->GetAnimationController()->SetNextAnim((int)SWORDBOSS_ANIMATION::RUN, 0.1f);

	OutputDebugString("Move\n");

}

void SwordBossMove::OnExit()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetAstarMoveAction()->StopMove();
}

//-------------------------------------Attack-------------------------------------------

SwordBossAttack::SwordBossAttack(StateManager* owner) : StateBase(owner), time_(0), nextAttack_(SWORDBOSS_ANIMATION::Slash_Up)
{
	pBoss_ = static_cast<SwordBoss*>(owner_->GetGameObject());
}

void SwordBossAttack::Update()
{
	time_++;

	//今の攻撃のUpdateを呼ぶ
	switch (nextAttack_)
	{
	case SWORDBOSS_ANIMATION::Slash_Up:		UpdateSlashUp();	 break;
	case SWORDBOSS_ANIMATION::Slash_Right:	UpdateSlashRight();	 break;
	case SWORDBOSS_ANIMATION::Slash_Jump:	UpdateSlashJump();	 break;
	case SWORDBOSS_ANIMATION::Thrust:		UpdateThrust();		 break;
	default: break;
	}

	//今実行中の攻撃が終了したから次の行動に移る
	int AnimFrame = pBoss_->GetAnimationController()->GetAnimTime((int)nextAttack_);
	if (time_ >= AnimFrame) {
		//コンボ記録に追加
		pBoss_->GetSelectAttack()->AddComboHistory(pBoss_->GetSelectAttack()->GetSelectAttack());
		
		//コンボできるならもう一度AttackState
		if (pBoss_->GetSelectAttack()->Selector(pBoss_)) owner_->ChangeState("Attack");

		//コンボできないからWaitStateに
		else owner_->ChangeState("Wait");
	}
}

void SwordBossAttack::OnEnter()
{
	//B-Treeかコンボで選択された攻撃を取得する
	int atkNumber = pBoss_->GetSelectAttack()->GetSelectAttack();
	switch (atkNumber) {
	case 0: nextAttack_ = SWORDBOSS_ANIMATION::Slash_Up;	break;
	case 1: nextAttack_ = SWORDBOSS_ANIMATION::Slash_Right;	break;
	case 2: nextAttack_ = SWORDBOSS_ANIMATION::Slash_Jump;	break;
	case 3: nextAttack_ = SWORDBOSS_ANIMATION::Thrust;		break;
	}

	//アニメーションの再生する
	pBoss_->GetAnimationController()->SetNextAnim((int)nextAttack_, 0.1f);

	//初期化
	time_ = 0;
	
	OutputDebugString("Attack\n");
}

void SwordBossAttack::OnExit()
{
	pBoss_->GetRotateAction()->SetRatio(ROTATE_RATIO);
	pBoss_->SetAttackCoolDown(rand() % 150);
	pBoss_->AttackEnd();
}

//------------------------------------- -------------------------------------------

void SwordBossAttack::UpdateSlashUp()
{
}

void SwordBossAttack::UpdateSlashRight()
{
}

void SwordBossAttack::UpdateSlashJump()
{
	if (time_ >= JumpMoveTime[0] && time_ <= JumpMoveTime[1]) {
		//ジャンプ移動
		float jumpParce = ((float)time_ - (float)JumpMoveTime[0]) / ((float)JumpMoveTime[1] - (float)JumpMoveTime[0]);
		float time = jumpParce * 2.0f - 1.0f;
		XMFLOAT3 pos = pBoss_->GetPosition();
		pos.y = (JumpheightPower * -time) + pos.y;
		pBoss_->SetPosition(pos);

		//近さで速さ抑制計算
		const float limitDist = 3.0f;
		float limitSpeed = 1.0f;
		float dist = CalculationDistance(GameManager::GetPlayer()->GetPosition(), pBoss_->GetPosition());
		if (dist < limitDist) limitSpeed *= (dist - pBoss_->GetBodyRange()) / limitDist;

		//移動
		pBoss_->GetOrientedMoveAction()->SetMoveSpeed(JumpMoveSpeed * limitSpeed);
		pBoss_->GetOrientedMoveAction()->SetDirection({ 0.0f, 0.0f, 1.0f, 0.0f });
		pBoss_->GetOrientedMoveAction()->Update();
	}
}

void SwordBossAttack::UpdateThrust()
{
	if (time_ >= ThrustMoveTime[0] && time_ <= ThrustMoveTime[1]) {
		//移動方向セット
		pBoss_->GetOrientedMoveAction()->SetDirection({ 0.0f, 0.0f, 1.0f, 0.0f });
		
		int AnimFrame = pBoss_->GetAnimationController()->GetAnimTime((int)nextAttack_);
		float t = (float)time_ / (float)ThrustMoveTime[1];
		t = Easing::EaseInQuint(t);
		pBoss_->GetOrientedMoveAction()->SetMoveSpeed(ThrustMoveSpeed * t);
		pBoss_->GetOrientedMoveAction()->Update();
	}
}

//--------------------------------------------------------------------------------
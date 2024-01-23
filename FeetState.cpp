#include "FeetState.h"
#include "StateManager.h"
#include "Player.h"
#include "Feet.h"
#include "CreateStage.h"
#include "GameManager.h"
#include "EnemyUi.h"
#include "Engine/Model.h"
#include "Engine/Global.h"
#include <vector>
#include "VFXManager.h"

#include "IsEnemyActionReady.h"
#include "ChangeStateNode.h"
#include "BehaviourNode.h"
#include "PlayerConditionNode.h"
#include "IsEnemyStateNode.h"
#include "MoveAction.h"
#include "RotateAction.h"
#include "SearchAction.h"
#include "StateCountNode.h"

namespace {
	const short foundSearch = 10;
}

FeetAppear::FeetAppear(StateManager* owner) : StateBase(owner), time_(0), appearTime_(0)
{
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetAppear::Update()
{
	time_++;
	if (time_ > appearTime_) owner_->ChangeState("Idle");

	float tsize = (float)time_ / (float)appearTime_;
	pFeet_->SetScale(XMFLOAT3(tsize, tsize, tsize));

}

void FeetAppear::OnEnter()
{
	appearTime_ = 180;

	XMFLOAT3 pos = pFeet_->GetPosition();
	VFXManager::CreatVfxEnemySpawn(pos);

}

void FeetAppear::OnExit()
{
	pFeet_->SetScale(XMFLOAT3(1, 1, 1));

}

//--------------------------------------------------------------------------------

FeetIdle::FeetIdle(StateManager* owner) : StateBase(owner)
{
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetIdle::Update()
{
	owner_->ChangeState("Patrol");
}

//--------------------------------------------------------------------------------

FeetPatrol::FeetPatrol(StateManager* owner) : StateBase(owner), foundSearchTime_(0)
{
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetPatrol::Update()
{
	//Astar�ړ����I������Ȃ�X�V�E�҂����ԓK����randam�� �f�o�b�O�p
	if (pFeet_->GetMoveAction()->IsInRange() && rand() % 60 == 0) {
		CreateStage* pCreateStage = GameManager::GetCreateStage();
		pFeet_->GetMoveAction()->UpdatePath(pCreateStage->GetRandomFloarPosition());
	}

	//Astar�ړ��E��]
	pFeet_->GetMoveAction()->Update();
	pFeet_->GetRotateAction()->Update();

	//FoundSearch�̎��s�҂����Ԃ�foundSearch
	foundSearchTime_++;
	if (foundSearchTime_ > foundSearch) {
		foundSearchTime_ = 0;
		pFeet_->GetVisionSearchAction()->Update();

		//����������CombatState�֐���
		if (pFeet_->GetVisionSearchAction()->IsFoundTarget()) {
			owner_->ChangeState("Combat");
		}
	}
}

void FeetPatrol::OnEnter()
{
	pFeet_->GetMoveAction()->SetMoveSpeed(0.02f);
	pFeet_->GetRotateAction()->SetTarget(nullptr);
}

void FeetPatrol::OnExit()
{
	pFeet_->GetMoveAction()->SetMoveSpeed(0.03f);
	pFeet_->GetMoveAction()->StopMove();
}

//--------------------------------------------------------------------------------

FeetCombat::FeetCombat(StateManager* owner) : StateBase(owner)
{
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());

	//-----------------------------�r�w�C�r�A�c���[�̐ݒ�--------------------------------------
	root_ = new Root();
	Selector* selector1 = new Selector();
	root_->SetRootNode(selector1);

	//---------------------------------------�U��State��Selector�̓o�^----------------------------
	Selector* selector2 = new Selector();
	IsNotEnemyCombatState* condition1 = new IsNotEnemyCombatState(selector2, "Attack", pFeet_);
	selector1->AddChildren(condition1);

	//--------------------Move�ֈڍs����------------------------------------
	EnemyChangeCombatStateNode* action1 = new EnemyChangeCombatStateNode(pFeet_, "Move");
	IsEnemyAttackReady* condition2 = new IsEnemyAttackReady(action1, pFeet_);

	//����AI��ConditionNode�i�U���\�ő吔�͈͓���Test
	CombatStateCountNode* conditionA = new CombatStateCountNode(condition2, 2, { "Move", "Attack" });
	IsEnemyCombatState* condition3 = new IsEnemyCombatState(conditionA, "Wait", pFeet_);
	selector2->AddChildren(condition3);

	//--------------------Attack�ֈڍs����-----------------------
	EnemyChangeCombatStateNode* action2 = new EnemyChangeCombatStateNode(pFeet_, "Attack");
	IsPlayerInRangeNode* condition4 = new IsPlayerInRangeNode(action2, 5.0f, pFeet_, GameManager::GetPlayer());
	IsEnemyCombatState* condition5 = new IsEnemyCombatState(condition4, "Move", pFeet_);
	selector2->AddChildren(condition5);

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
	pFeet_->GetRotateAction()->SetTarget(GameManager::GetPlayer());

}

FeetCombat::~FeetCombat()
{
	delete root_;
}

//--------------------------------------------------------------------------------

FeetDead::FeetDead(StateManager* owner) : StateBase(owner)
{
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetDead::Update()
{
}

//-------------------------------------CombatState-------------------------------------------

FeetWait::FeetWait(StateManager* owner) : StateBase(owner)
{
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetWait::Update()
{
	if (rand() % 100 == 0) {
		if (pFeet_->GetMoveAction()->IsInRange() || pFeet_->GetMoveAction()->IsOutTarget()) {
			CreateStage* pCreateStage = GameManager::GetCreateStage();
			pFeet_->GetMoveAction()->UpdatePath(pCreateStage->GetFloarPosition(pFeet_->GetPosition(), 10.0f));
		}
	}

	pFeet_->GetMoveAction()->Update();
	pFeet_->GetRotateAction()->Update();

}

void FeetWait::OnEnter()
{
	OutputDebugString("WaitState\n");

	pFeet_->GetMoveAction()->SetMoveSpeed(0.02f);
}

//--------------------------------------------------------------------------------

FeetMove::FeetMove(StateManager* owner) : StateBase(owner)
{
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetMove::Update()
{
	//��񂄂�߂悤
	pFeet_->GetMoveAction()->SetTarget(GameManager::GetPlayer()->GetPosition());
	if (pFeet_->GetMoveAction()->IsInRange() && rand() % 10 == 0) {
		pFeet_->GetMoveAction()->UpdatePath(GameManager::GetPlayer()->GetPosition());
	}

	if (pFeet_->GetMoveAction()->IsOutTarget()) {
		pFeet_->GetMoveAction()->UpdatePath(GameManager::GetPlayer()->GetPosition());
	}

	pFeet_->GetMoveAction()->Update();
	pFeet_->GetRotateAction()->Update();
}

void FeetMove::OnEnter()
{
	OutputDebugString("MoveState\n");

	pFeet_->GetMoveAction()->SetMoveSpeed(0.03f);

}

void FeetMove::OnExit()
{
	pFeet_->GetMoveAction()->StopMove();
}

//--------------------------------------------------------------------------------

FeetAttack::FeetAttack(StateManager* owner) : StateBase(owner), time_(0)
{
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetAttack::Update()
{
	time_++;

	if (time_ < 30) pFeet_->GetRotateAction()->Update();

	//AttackFrame=65 �` 90
	//Feet��OnAttackCollision�̕���Player�ɓ���������false�ɂ��鏈���������Ă���
	if (time_ == 65) pFeet_->GetSphereCollider()->SetValid(true);
	if (time_ == 90) pFeet_->GetSphereCollider()->SetValid(false);

	if (time_ >= 200) {
		Model::SetAnimFrame(pFeet_->GetModelHandle(), 0, 0, 1.0f);
		owner_->ChangeState("Wait");
		return;
	}
}

void FeetAttack::OnEnter()
{
	OutputDebugString("AttackState\n");

	time_ = 0;
	Model::SetAnimFrame(pFeet_->GetModelHandle(), 0, 200, 1.0f);
}

//--------------------------------------------------------------------------------

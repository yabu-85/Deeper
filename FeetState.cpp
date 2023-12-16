#include "FeetState.h"
#include "StateManager.h"
#include "Player.h"
#include "Feet.h"
#include "Stage.h"
#include "DamageManager.h"
#include "GameManager.h"
#include "EnemyUi.h"
#include "Engine/Model.h"
#include "Engine/Global.h"

#include "ChangeStateNode.h"
#include "BehaviourNode.h"
#include "PlayerConditionNode.h"
#include "IsEnemyStateNode.h"
#include "MoveAction.h"
#include "RotateAction.h"
#include "SearchAction.h"

FeetAppear::FeetAppear(StateManager* owner) : StateBase(owner), time_(0), appearTime_(0)
{
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetAppear::Update()
{
	time_++;
	if (time_ > appearTime_) owner_->ChangeState("Idle");

}

void FeetAppear::Initialize()
{
	appearTime_ = 60;
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

namespace {
	const short foundSearch = 10;
}

FeetPatrol::FeetPatrol(StateManager* owner) : StateBase(owner), foundSearchTime_(0)
{
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetPatrol::Update()
{
	//Astar�ړ����I������Ȃ�X�V�E�҂����ԓK����randam�� �f�o�b�O�p
	if (pFeet_->GetMoveAction()->IsInRange() && rand() % 60 == 0) {
		Stage* pStage = (Stage*)pFeet_->FindObject("Stage");
		pFeet_->GetMoveAction()->SetTarget(pStage->GetRandomFloarPosition());
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
	pFeet_->GetMoveAction()->SetMoveSpeed(0.07f);
	pFeet_->GetMoveAction()->StopMove();
}

//--------------------------------------------------------------------------------

FeetCombat::FeetCombat(StateManager* owner) : StateBase(owner)
{
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
	Player* pPlayer = static_cast<Player*>(pFeet_->FindObject("Player"));

	//-----------------------------�r�w�C�r�A�c���[�̐ݒ�--------------------------------------
	root_ = new Root();
	Selector* selector1 = new Selector();
	root_->SetRootNode(selector1);

	//---------------------------------------�U���\------------------------------------------------
	Selector* selector2 = new Selector();
	IsNotEnemyCombatState* condition1 = new IsNotEnemyCombatState(selector2, "Attack", pFeet_);
	Inverter* inverter1 = new Inverter(condition1);
	selector1->AddChildren(inverter1);
	
	//--------------------�v���C���[�̋߂��ł͂Ȃ��i�ړ��j/�@�߂��Ȃ�U����I��-----------------------
	EnemyChangeCombatStateNode* action1 = new EnemyChangeCombatStateNode(pFeet_, "Move");
	IsPlayerNotInRangeNode* condition2 = new IsPlayerNotInRangeNode(action1, 5.0f, pFeet_, pPlayer);
	selector2->AddChildren(condition2);

	Selector* selector3 = new Selector();
	selector2->AddChildren(selector3);

	//--------------------------------�U���̂ǂꂩ��I��-------------------------------------------
	EnemyChangeCombatStateNode* action2 = new EnemyChangeCombatStateNode(pFeet_, "Attack");
	selector3->AddChildren(action2);

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
}

//--------------------------------------------------------------------------------

FeetMove::FeetMove(StateManager* owner) : StateBase(owner)
{
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetMove::Update()
{
	//rand() �ɂ��Ă邯�ǂȂ񂩂���Ă����Ƃ����������ق���������
	if (pFeet_->GetMoveAction()->IsInRange() && rand() % 10 == 0) {
		Player* pPlayer = (Player*)pFeet_->FindObject("Player");
		pFeet_->GetMoveAction()->SetTarget(pPlayer->GetPosition());
	}

	pFeet_->GetMoveAction()->Update();
	pFeet_->GetRotateAction()->Update();
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

	//AttackFrame=65 �` 90
	if (time_ > 65 && time_ < 90 && !pFeet_->GetAttackColliderList().empty()) {
		GameManager::GetDamageManager()->CalcPlyaer(pFeet_->GetSphereCollider(), 1);
	}
	
	if (time_ >= 200) {
		Model::SetAnimFrame(pFeet_->GetModelHandle(), 0, 0, 1.0f);
		owner_->ChangeState("Wait");
		return;
	}
}

void FeetAttack::OnEnter()
{
	time_ = 0;
	Model::SetAnimFrame(pFeet_->GetModelHandle(), 0, 200, 1.0f);
}

//--------------------------------------------------------------------------------

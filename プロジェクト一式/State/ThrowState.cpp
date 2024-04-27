#include "ThrowState.h"
#include "StateManager.h"
#include "../GameManager/GameManager.h"
#include "../Player/Player.h"
#include "../Enemy/ThrowEnemy.h"
#include "../Stage/CreateStage.h"
#include "../Enemy/EnemyUi.h"
#include "../Engine/Model.h"
#include "../Engine/Global.h"
#include "../Other/VFXManager.h"

#include "../BehaviorTree/IsEnemyActionReadyNode.h"
#include "../BehaviorTree/ChangeStateNode.h"
#include "../BehaviorTree/BehaviourNode.h"
#include "../BehaviorTree/PlayerConditionNode.h"
#include "../BehaviorTree/IsEnemyStateNode.h"
#include "../BehaviorTree/IsEnemyPermission.h"
#include "../BehaviorTree/CameraConditionNode.h"

#include "../Action/MoveAction.h"
#include "../Action/RotateAction.h"
#include "../Action/SearchAction.h"

namespace {
	static const int FOUND_SEARCH = 10;		//���o�T�m�̍X�V����
	static const int APPER_TIME = 180;
	static const float FAST_SPEED = 0.03f;
	static const float SLOW_SPEED = 0.02f;
	static const int DEAD_TIME = 100;

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
	VFXManager::CreateVfxEnemySpawn(pos);

}

void ThrowAppear::OnExit()
{
	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	e->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));

}

//--------------------------------------------------------------------------------

void ThrowDead::Update()
{
	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	time_++;

	float s = (float)time_ / (float)DEAD_TIME;
	s = (1.0f - s) * 0.8f;
	e->SetScale({ s, s, s });

	if (time_ >= DEAD_TIME) e->DeadExit();
}

void ThrowDead::OnEnter()
{
	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	e->DeadEnter();
	time_ = 0;
}

//--------------------------------------------------------------------------------

void ThrowPatrol::Update()
{
	//Astar�ړ����I������Ȃ�X�V�E�҂����ԓK����randam�� �f�o�b�O�p
	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	if (e->GetMoveAction()->IsInRange() && rand() % 60 == 0) {
		e->GetMoveAction()->UpdatePath(GameManager::GetCreateStage()->GetRandomFloarPosition());
	}

	//Astar�ړ��E��]
	e->GetMoveAction()->Update();
	e->GetRotateAction()->Update();

	//FoundSearch�̎��s�҂����Ԃ�foundSearch
	time_++;
	if (time_ > FOUND_SEARCH) {
		time_ = 0;
		e->GetVisionSearchAction()->Update();

		//����������CombatState�֐���
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
	
	//-----------------------------�r�w�C�r�A�c���[�̐ݒ�--------------------------------------
	root_ = new Root();
	Selector* selector1 = new Selector();
	root_->SetRootNode(selector1);

	Selector* waitSelector = new Selector();
	Selector* moveSelector = new Selector();
	IsEnemyCombatState* wCon = new IsEnemyCombatState(waitSelector, "Wait", e);
	IsEnemyCombatState* mCon = new IsEnemyCombatState(moveSelector, "Move", e);
	selector1->AddChildren(wCon);
	selector1->AddChildren(mCon);

	//--------------------Move�ֈڍs����------------------------------------
	EnemyChangeCombatStateNode* action3 = new EnemyChangeCombatStateNode(e, "Attack");
	IsEnemyAttackPermission* condition5 = new IsEnemyAttackPermission(action3, e);
	IsPlayerInRangeNode* condition6 = new IsPlayerInRangeNode(condition5, e->GetAttackDistance(), e, GameManager::GetPlayer());
	IsEnemyWithinScreen* condition8 = new IsEnemyWithinScreen(condition6, e);
	waitSelector->AddChildren(condition8);

	EnemyChangeCombatStateNode* action1 = new EnemyChangeCombatStateNode(e, "Move");
	IsEnemyMovePermission* condition2 = new IsEnemyMovePermission(action1, e);
	IsEnemyActionReady* condition3 = new IsEnemyActionReady(condition2, e);
	waitSelector->AddChildren(condition3);

	//--------------------Attack�ֈڍs����-----------------------
	moveSelector->AddChildren(condition8);

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

void ThrowWait::Update()
{
	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());

	//Player������̋����ړ�������
	if (rand() % 10 == 0) {
		//�v���C���[�̕����Ńv���C���[�Ƃ̋�����CombadDistance�ɂȂ�悤�ړ�����
		XMFLOAT3 pos = GameManager::GetCreateStage()->GetPositionPlayerDirection(e->GetPosition(), e->GetCombatDistance());
		e->GetMoveAction()->UpdatePath(pos);
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

void ThrowMove::Update()
{
	//�v���C���[�̏ꏊ�Ɉړ�
	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	if (rand() % 10 == 0) {
		
		//�v���C���[�̕����Ńv���C���[�Ƃ̋�����CombadDistance�ɂȂ�悤�ړ�����
		XMFLOAT3 pos = GameManager::GetCreateStage()->GetPositionPlayerDirection(e->GetPosition(), e->GetCombatDistance());
		e->GetMoveAction()->UpdatePath(pos);
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

}

void ThrowAttack::OnExit()
{
	ThrowEnemy* e = static_cast<ThrowEnemy*>(owner_->GetGameObject());
	e->SetAttackCoolDown(rand() % 100);

}

//--------------------------------------------------------------------------------
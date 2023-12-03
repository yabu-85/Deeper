#include "Feet.h"
#include "Engine/Model.h"
#include "EnemyUi.h"
#include "Engine/SphereCollider.h"
#include "StateManager.h"
#include "FeetState.h"

#include "MoveAction.h"
#include "RotateAction.h"

Feet::Feet(GameObject* parent)
	:EnemyBase(parent), hModel_(-1), pHandCollider_(nullptr), pMoveAction_(nullptr), pRotateAction_(nullptr)
{
	objectName_ = "Feet";
}

Feet::~Feet()
{
	Release();
}

void Feet::Initialize()
{
	//���f���f�[�^�̃��[�h
	hModel_ = Model::Load("Model/StoneGolem.fbx");
	assert(hModel_ >= 0);
//	Model::SetAnimFrame(hModel_, 0, 100, 1.0f);

	transform_.rotate_.y = -90.0f;
	transform_.scale_ = XMFLOAT3(2.0f, 2.0f, 2.0f);
	transform_.position_ = XMFLOAT3(50.0f + (float)(rand() % 10), 0.0f, 50.0f + (float)(rand() % 10));

	maxHp_ = 10;
	hp_ = maxHp_;
	aimTargetPos_ = 3.0f;

	//Collider�̐ݒ�
	SphereCollider* collision1 = new SphereCollider(XMFLOAT3(0, 3, 0), 1.5f);
	SphereCollider* collision2 = new SphereCollider(XMFLOAT3(0, 1, 0), 1.5f);
	pHandCollider_ = new SphereCollider(XMFLOAT3(0, 0, 0), 1.0f);
	AddCollider(collision1);
	AddCollider(collision2);
	AddAttackCollider(pHandCollider_);
	
	//�X�e�[�g�̐ݒ�
	pStateManager_ = new StateManager(this);
	pStateManager_->AddState(new FeetAppear(pStateManager_));
	pStateManager_->AddState(new FeetIdle(pStateManager_));
	pStateManager_->AddState(new FeetPatrol(pStateManager_));
	pStateManager_->AddState(new FeetCombat(pStateManager_));
	pStateManager_->AddState(new FeetDead(pStateManager_));
	pStateManager_->ChangeState("Appear");
	pStateManager_->Initialize();
	
	//CombatState�̐ݒ�
	pCombatStateManager_ = new StateManager(this);
	pCombatStateManager_->AddState(new FeetWait(pCombatStateManager_));
	pCombatStateManager_->AddState(new FeetMove(pCombatStateManager_));
	pCombatStateManager_->AddState(new FeetAttack(pCombatStateManager_));
	pCombatStateManager_->ChangeState("Wait");
	pCombatStateManager_->Initialize();
	
	pEnemyUi_ = new EnemyUi(this);
	pEnemyUi_->Initialize(5.5f);

	//Action�̐ݒ�
	pMoveAction_ = new AstarMoveAction(this, 0.07f, 2.0f);
	pRotateAction_ = new RotateAction(this, 0.07f);
	pMoveAction_->Initialize();
	pRotateAction_->Initialize();

}

void Feet::Update()
{
	if(pStateManager_) pStateManager_->Update();
	if(pEnemyUi_) pEnemyUi_->Update();

}

void Feet::Draw()
{
	XMFLOAT3 center = Model::GetBoneAnimPosition(hModel_, "hand.R");
	center = XMFLOAT3(center.x - transform_.position_.x, center.y - transform_.position_.y, center.z - transform_.position_.z);
	pHandCollider_->SetCenter(center);

	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	CollisionDraw();
}

void Feet::Release()
{
}
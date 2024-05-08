#include "MeleeFighter.h"
#include "EnemyUi.h"
#include "../GameManager/GameManager.h"
#include "../Engine/Model.h"
#include "../Engine/SphereCollider.h"
#include "../Engine/Global.h"
#include "../Engine/PolyLine.h"
#include "../State/StateManager.h"
#include "../State/MeleeFighterState.h"
#include "../Stage/CreateStage.h"
#include "../Stage/CollisionMap.h"
#include "../Animation/AnimationController.h"

#include "../Action/MoveAction.h"
#include "../Action/RotateAction.h"
#include "../Action/SearchAction.h"

namespace {
	static const int POLY_SMOOTH = 0;
	static const int POLY_DRAW_TIME = 20;

}

MeleeFighter::MeleeFighter(GameObject* parent)
	: EnemyBase(parent, "MeleeFighterEnemy"), hModel_(-1), pMoveAction_(nullptr), pRotateAction_(nullptr), pVisionSearchAction_(nullptr),
	pOrientedMoveAction_(nullptr), boneIndex_{ -1,-1 }, partIndex_{ -1,-1 }, pDamageController_(nullptr), pAnimationController_(nullptr), 
	pPolyLine_{ nullptr, nullptr }
{
}

MeleeFighter::~MeleeFighter()
{
}

void MeleeFighter::Initialize()
{	
	hModel_ = Model::Load("Model/walf.fbx");
	assert(hModel_ >= 0);
	
	Model::GetBoneIndex(hModel_, "attack_Hand.R", &boneIndex_[0], &partIndex_[0]);
	assert(boneIndex_[0] >= 0);
	Model::GetBoneIndex(hModel_, "attack_Hand.L", &boneIndex_[1], &partIndex_[1]);
	assert(boneIndex_[1] >= 0);

	transform_.position_ = GameManager::GetCreateStage()->GetRandomFloarPosition();
	transform_.rotate_.y = (float)(rand() % 360);

	SetHP(100);
	SetMaxHP(100);
	SetBodyWeight(10.0f);
	SetBodyRange(0.35f);

	type_ = ENEMY_MELEE;
	aimTargetPos_ = 0.8f;
	attackDistance_ = 2.0f;
	combatDistance_ = 5.0f;

	//�A�j���[�V�����f�[�^�̃Z�b�g�t���[���̓w�b�_�ɏ����Ă�
	pAnimationController_ = new AnimationController(hModel_, this);
	for (int i = 0; i < (int)MELEE_ANIMATION::MAX; i++) pAnimationController_->AddAnim(MELEE_ANIMATION_DATA[i][0], MELEE_ANIMATION_DATA[i][1]);

	//Collider�̐ݒ�
	AddCollider(new SphereCollider(XMFLOAT3(0, 0.5f, 0), 0.35f));
	AddCollider(new SphereCollider(XMFLOAT3(0, 1.1f, 0), 0.35f));
	AddAttackCollider(new SphereCollider(XMFLOAT3(0, 0, 0), 0.25f));
	AddAttackCollider(new SphereCollider(XMFLOAT3(0, 0, 0), 0.25f));
	SetAllAttackColliderValid(false);

	pDamageController_ = new DamageController();
	pEnemyUi_ = new EnemyUi(this);
	pEnemyUi_->Initialize(1.9f);

	//Action�̐ݒ�
	pMoveAction_ = new AstarMoveAction(this, 0.0f, 0.3f);
	pOrientedMoveAction_ = new OrientedMoveAction(this, 0.02f);
	pRotateAction_ = new RotateAction(this, 0.0f);
	pVisionSearchAction_ = new VisionSearchAction(this, 30.0f, 90.0f);
	pRotateAction_->Initialize();

	//�X�e�[�g�̐ݒ�
	pStateManager_ = new StateManager(this);
	pStateManager_->AddState(new MeleeFighterAppear(pStateManager_));
	pStateManager_->AddState(new MeleeFighterDead(pStateManager_));
	pStateManager_->AddState(new MeleeFighterPatrol(pStateManager_));
	pStateManager_->AddState(new MeleeFighterCombat(pStateManager_));
	pStateManager_->ChangeState("Appear");
	pStateManager_->Initialize();
	
	//CombatState�̐ݒ�
	pCombatStateManager_ = new StateManager(this);
	pCombatStateManager_->AddState(new MeleeFighterWait(pCombatStateManager_));
	pCombatStateManager_->AddState(new MeleeFighterMove(pCombatStateManager_));
	pCombatStateManager_->AddState(new MeleeFighterAttack(pCombatStateManager_));
	pCombatStateManager_->ChangeState("Wait");
	pCombatStateManager_->Initialize();

	for (int i = 0; i < 2; i++) {
		pPolyLine_[i] = new PolyLine;
		pPolyLine_[i]->Load("PolyImage/Burger.png");
		pPolyLine_[i]->SetLength(POLY_DRAW_TIME);
		pPolyLine_[i]->SetSmooth(POLY_SMOOTH);
	}
}

void MeleeFighter::Update()
{
	EnemyBase::Update();
	pAnimationController_->Update();
	pStateManager_->Update();	
	GameManager::GetCollisionMap()->CalcMapWall(transform_.position_, 0.1f, GetBodyRange());
}

void MeleeFighter::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	CollisionDraw();
	pEnemyUi_->Draw();

	pMoveAction_->Draw();

	for (int i = 0; i < 2; i++) pPolyLine_[i]->Draw();
}

void MeleeFighter::Release()
{
	for (int i = 0; i < 2; i++) {
		SAFE_RELEASE(pPolyLine_[i]);
		SAFE_DELETE(pPolyLine_[i]);
	}
	
	SAFE_DELETE(pVisionSearchAction_);
	SAFE_DELETE(pRotateAction_);
	SAFE_DELETE(pOrientedMoveAction_);
	SAFE_DELETE(pMoveAction_);
	SAFE_DELETE(pAnimationController_);
	SAFE_DELETE(pDamageController_);

	EnemyBase::Release();
	Model::Release(hModel_);

}

void MeleeFighter::OnAttackCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player") {
		Character* p = static_cast<Character*>(pTarget);
		DamageInfo damage(GetDamageController()->GetCurrentDamage());
		KnockBackInfo knock(GetDamageController()->GetCurrentKnockBackInfo());
		knock.pos = this->GetPosition();

		//�U���������烊�X�g�ɒǉ�
		if (p->ApplyDamageWithList(damage, knock)) {
			GetDamageController()->AddAttackList(p);
		}
	}
}

void MeleeFighter::CalcAttack()
{
	std::list<Collider*> list = GetAttackColliderList();
	auto it = list.begin();
	for (int i = 0; i < 2; i++) {
		XMFLOAT3 wandPos = Model::GetBoneAnimPosition(hModel_, boneIndex_[i], partIndex_[i]);
		pPolyLine_[i]->AddPosition(wandPos);

		//Attack�R���W�����̍��W�ݒ�
		XMFLOAT3 center = XMFLOAT3(wandPos.x - transform_.position_.x, wandPos.y - transform_.position_.y, wandPos.z - transform_.position_.z);
		(*it)->SetCenter(center);
	}
}

void MeleeFighter::AttackEnd()
{
	SetAllAttackColliderValid(false);
	GetDamageController()->ResetAttackList();
	for (int i = 0; i < 2; i++) pPolyLine_[i]->ResetPosition();
}

void MeleeFighter::SetDamageInfo1()
{
	DamageInfo damage(this, "MeleeAttack", 3);
	KnockBackInfo knockBack(KNOCK_TYPE::MEDIUM, 0, 0.0f, transform_.position_);
	GetDamageController()->SetCurrentDamage(damage);
	GetDamageController()->SetCurrentKnockBackInfo(knockBack);
}
#include "MeleeFighter.h"
#include "EnemyUi.h"
#include "../GameManager/GameManager.h"
#include "../Engine/Model.h"
#include "../Engine/SphereCollider.h"
#include "../Engine/Global.h"
#include "../State/StateManager.h"
#include "../State/MeleeFighterState.h"
#include "../Stage/CreateStage.h"
#include "../Stage/CollisionMap.h"
#include "../Player/Player.h"
#include "../Player/LifeManager.h"

#include "../Action/MoveAction.h"
#include "../Action/RotateAction.h"
#include "../Action/SearchAction.h"

MeleeFighter::MeleeFighter(GameObject* parent)
	: EnemyBase(parent, "MeleeFighterEnemy"), hModel_(-1), pHandCollider_{ nullptr, nullptr }, pMoveAction_(nullptr), pRotateAction_(nullptr),
	pVisionSearchAction_(nullptr), pOrientedMoveAction_(nullptr), boneIndex_{ -1,-1 }, partIndex_{ -1,-1 }
{
}

MeleeFighter::~MeleeFighter()
{
}

void MeleeFighter::Initialize()
{
	hModel_ = Model::Load("Model/Fiter3.fbx");
	assert(hModel_ >= 0);

	CreateStage* pCreateStage = GameManager::GetCreateStage();
	XMFLOAT3 startPos = pCreateStage->GetRandomFloarPosition();
	transform_.position_ = startPos;
	transform_.rotate_.y = (float)(rand() % 360);

	type_ = ENEMY_MELEE;
	maxHp_ = 100;
	hp_ = maxHp_;
	aimTargetPos_ = 1.3f;
	bodyWeight_ = 100.0f;
	bodyRange_ = 0.7f;
	attackDamage_ = 1;
	combatDistance_ = 5.0f;

	//Colliderの設定
	SphereCollider* collision1 = new SphereCollider(XMFLOAT3(0, 0.5, 0), 0.75f);
	SphereCollider* collision2 = new SphereCollider(XMFLOAT3(0, 1.5, 0), 0.75f);
	pHandCollider_[0] = new SphereCollider(XMFLOAT3(0, 0, 0), 0.5f);
	pHandCollider_[1] = new SphereCollider(XMFLOAT3(0, 0, 0), 0.3f);
	for (int i = 0; i < 2; i++) {
		pHandCollider_[i]->SetValid(false);
		AddAttackCollider(pHandCollider_[i]);
	}
	AddCollider(collision1);
	AddCollider(collision2);

	pEnemyUi_ = new EnemyUi(this);
	pEnemyUi_->Initialize(3.0f);

	//Actionの設定
	pMoveAction_ = new AstarMoveAction(this, 0.0f, 0.3f);
	pOrientedMoveAction_ = new OrientedMoveAction(this, 0.02f);
	pRotateAction_ = new RotateAction(this, 0.0f);
	pVisionSearchAction_ = new VisionSearchAction(this, 30.0f, 90.0f);
	pRotateAction_->Initialize();

	//ステートの設定
	pStateManager_ = new StateManager(this);
	pStateManager_->AddState(new MeleeFighterAppear(pStateManager_));
	pStateManager_->AddState(new MeleeFighterDead(pStateManager_));
	pStateManager_->AddState(new MeleeFighterPatrol(pStateManager_));
	pStateManager_->AddState(new MeleeFighterCombat(pStateManager_));
	pStateManager_->ChangeState("Appear");
	pStateManager_->Initialize();
	
	//CombatStateの設定
	pCombatStateManager_ = new StateManager(this);
	pCombatStateManager_->AddState(new MeleeFighterWait(pCombatStateManager_));
	pCombatStateManager_->AddState(new MeleeFighterMove(pCombatStateManager_));
	pCombatStateManager_->AddState(new MeleeFighterAttack(pCombatStateManager_));
	pCombatStateManager_->ChangeState("Wait");
	pCombatStateManager_->Initialize();
	
	Model::GetBoneIndex(hModel_, "attack_Hand.R", &boneIndex_[0], &partIndex_[0]);
	//assert(boneIndex_[0] >= 0);
	Model::GetBoneIndex(hModel_, "forearm.R", &boneIndex_[1], &partIndex_[1]);
	//assert(boneIndex_[1] >= 0);

}

void MeleeFighter::Update()
{
	EnemyBase::Update();

	pStateManager_->Update();
	GameManager::GetCollisionMap()->CalcMapWall(transform_.position_, 0.1f);

}

void MeleeFighter::Draw()
{
	pEnemyUi_->Draw();

	//ColliderPosition
	for (int i = 0; i < 2; i++) {
	//	XMFLOAT3 center = Model::GetBoneAnimPosition(hModel_, boneIndex_[i], partIndex_[i]);
	//	center = XMFLOAT3(center.x - transform_.position_.x, center.y - transform_.position_.y, center.z - transform_.position_.z);
	//	pHandCollider_[i]->SetCenter(center);
	}	

	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	//デバッグ用
	CollisionDraw();

}

void MeleeFighter::Release()
{
	SAFE_DELETE(pVisionSearchAction_);
	SAFE_DELETE(pRotateAction_);
	SAFE_DELETE(pMoveAction_);

	EnemyBase::Release();
	Model::Release(hModel_);

}

void MeleeFighter::ApplyDamage(int da)
{
	EnemyBase::ApplyDamage(da);
	//死んでたら終わり
	if (pStateManager_->GetName() == "Dead") return;

	if (pStateManager_->GetName() != "Combat") {
		pStateManager_->ChangeState("Combat");
	}

}

void MeleeFighter::OnAttackCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player") {
		GameManager::GetPlayer()->TargetRotate(GetPosition());
		LifeManager::Damage(GetAttackDamage());
	}
}

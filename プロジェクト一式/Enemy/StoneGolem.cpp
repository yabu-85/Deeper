#include "StoneGolem.h"
#include "EnemyUi.h"
#include "../GameManager/GameManager.h"
#include "../Engine/Model.h"
#include "../Engine/SphereCollider.h"
#include "../Engine/Global.h"
#include "../State/StateManager.h"
#include "../State/StoneGolemState.h"
#include "../Stage/CreateStage.h"
#include "../Stage/CollisionMap.h"

#include "../Action/MoveAction.h"
#include "../Action/RotateAction.h"
#include "../Action/SearchAction.h"

namespace {
	KnockBackInfo knockBack1(KNOCK_TYPE::MEDIUM, 6, 0.2f, XMFLOAT3());

}

StoneGolem::StoneGolem(GameObject* parent)
	: EnemyBase(parent, "StoneGolemEnemy"), hModel_(-1), pMoveAction_(nullptr), pRotateAction_(nullptr), pDamageController_(nullptr),
	pVisionSearchAction_(nullptr), pOrientedMoveAction_(nullptr), boneIndex_{ -1,-1 }, partIndex_{ -1,-1 }
{
}

StoneGolem::~StoneGolem()
{
}

void StoneGolem::Initialize()
{
	hModel_ = Model::Load("Model/stoneGolem.fbx");
	assert(hModel_ >= 0);

	CreateStage* pCreateStage = GameManager::GetCreateStage();
	XMFLOAT3 startPos = pCreateStage->GetRandomFloarPosition(0.7f);
	transform_.position_ = startPos;
	transform_.rotate_.y = (float)(rand() % 360);

	SetHP(200);
	SetMaxHP(200);
	SetBodyWeight(100.0f);
	SetBodyRange(0.7f);
	
	type_ = ENEMY_STONEGOLEM;
	aimTargetPos_ = 1.0f;
	attackDistance_ = 2.0f;
	combatDistance_ = 5.0f;

	//Colliderの設定
	SphereCollider* collision1 = new SphereCollider(XMFLOAT3(0, 0.5, 0), 0.75f);
	SphereCollider* collision2 = new SphereCollider(XMFLOAT3(0, 1.5, 0), 0.75f);
	SphereCollider* collision3 = new SphereCollider(XMFLOAT3(0, 0, 0), 0.5f);
	SphereCollider* collision4 = new SphereCollider(XMFLOAT3(0, 0, 0), 0.3f);
	AddCollider(collision1);
	AddCollider(collision2);
	AddAttackCollider(collision3);
	AddAttackCollider(collision4);
	SetAllAttackColliderValid(false);

	pEnemyUi_ = new EnemyUi(this);
	pEnemyUi_->Initialize(2.8f);

	//Actionの設定
	pMoveAction_ = new AstarMoveAction(this, 0.0f, 0.3f);
	pOrientedMoveAction_ = new OrientedMoveAction(this, 0.02f);
	pRotateAction_ = new RotateAction(this, 0.0f);
	pVisionSearchAction_ = new VisionSearchAction(this, 30.0f, 90.0f);
	pRotateAction_->Initialize();
	
	pDamageController_ = new DamageController();

	//ステートの設定
	pStateManager_ = new StateManager(this);
	pStateManager_->AddState(new StoneGolemAppear(pStateManager_));
	pStateManager_->AddState(new StoneGolemDead(pStateManager_));
	pStateManager_->AddState(new StoneGolemPatrol(pStateManager_));
	pStateManager_->AddState(new StoneGolemCombat(pStateManager_));
	pStateManager_->ChangeState("Appear");
	pStateManager_->Initialize();
	
	//CombatStateの設定
	pCombatStateManager_ = new StateManager(this);
	pCombatStateManager_->AddState(new StoneGolemWait(pCombatStateManager_));
	pCombatStateManager_->AddState(new StoneGolemMove(pCombatStateManager_));
	pCombatStateManager_->AddState(new StoneGolemAttack(pCombatStateManager_));
	pCombatStateManager_->ChangeState("Wait");
	pCombatStateManager_->Initialize();
	
	Model::GetBoneIndex(hModel_, "attack_Hand.R", &boneIndex_[0], &partIndex_[0]);
	assert(boneIndex_[0] >= 0);
	Model::GetBoneIndex(hModel_, "forearm.R", &boneIndex_[1], &partIndex_[1]);
	assert(boneIndex_[1] >= 0);

}

void StoneGolem::Update()
{
	EnemyBase::Update();

	pStateManager_->Update();
	GameManager::GetCollisionMap()->CalcMapWall(transform_.position_, 0.1f, GetBodyRange());

}

void StoneGolem::Draw()
{
	std::list<Collider*> list = GetAttackColliderList();
	auto it = list.begin();
	for (int i = 0; i < 2; i++) {
		XMFLOAT3 center = Model::GetBoneAnimPosition(hModel_, boneIndex_[i], partIndex_[i]);
		center = XMFLOAT3(center.x - transform_.position_.x, center.y - transform_.position_.y, center.z - transform_.position_.z);
		
		(*it)->SetCenter(center);
		it++;
	}

	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	pMoveAction_->Draw();

	CollisionDraw();
	pEnemyUi_->Draw();

}

void StoneGolem::Release()
{
	SAFE_DELETE(pVisionSearchAction_);
	SAFE_DELETE(pRotateAction_);
	SAFE_DELETE(pOrientedMoveAction_);
	SAFE_DELETE(pMoveAction_);
	SAFE_DELETE(pDamageController_);

	EnemyBase::Release();
	Model::Release(hModel_);

}

void StoneGolem::OnAttackCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player") {
		Character* p = static_cast<Character*>(pTarget);
		DamageInfo damage(GetDamageController()->GetCurrentDamage());
		KnockBackInfo knock(GetDamageController()->GetCurrentKnockBackInfo());
		knock.pos = this->GetPosition();

		//攻撃入ったらリストに追加
		if (p->ApplyDamageWithList(damage, knock)) {
			GetDamageController()->AddAttackList(p);
		}
	}
}

void StoneGolem::DamageInfoReset()
{
	SetAllAttackColliderValid(false);
	GetDamageController()->ResetAttackList();
}

void StoneGolem::SetDamageInfoCombo1()
{
	DamageInfo damage(this, "StoneArm", 3);
	KnockBackInfo knockBack(KNOCK_TYPE::MEDIUM, 6, 0.2f, transform_.position_);
	SetAllAttackColliderValid(true);
	GetDamageController()->SetCurrentDamage(damage);
	GetDamageController()->SetCurrentKnockBackInfo(knockBack);
}

void StoneGolem::SetDamageInfoCombo2()
{
	DamageInfo damage(this, "StoneArm", 3);
	KnockBackInfo knockBack(KNOCK_TYPE::MEDIUM, 6, 0.2f, transform_.position_);
	SetAllAttackColliderValid(true);
	GetDamageController()->SetCurrentDamage(damage);
	GetDamageController()->SetCurrentKnockBackInfo(knockBack);
}

void StoneGolem::SetDamageInfoCombo3()
{
	DamageInfo damage(this, "StoneArm", 5);
	KnockBackInfo knockBack(KNOCK_TYPE::LARGE, 20, 0.3f, { 0.f, 0.f, 0.f });
	SetAllAttackColliderValid(true);
	GetDamageController()->SetCurrentDamage(damage);
	GetDamageController()->SetCurrentKnockBackInfo(knockBack);
}

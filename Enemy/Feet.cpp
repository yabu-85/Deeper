#include "Feet.h"
#include "../Engine/Model.h"
#include "EnemyUi.h"
#include "../Engine/SphereCollider.h"
#include "../State/StateManager.h"
#include "../State/FeetState.h"
#include "../Stage/CreateStage.h"
#include "../Engine/Global.h"
#include "../GameManager.h"
#include "../Stage/CollisionMap.h"

#include "../Action/MoveAction.h"
#include "../Action/RotateAction.h"
#include "../Action/SearchAction.h"

Feet::Feet(GameObject* parent)
	: EnemyBase(parent, "FeetEnemy"), hModel_(-1), pHandCollider_(nullptr), pMoveAction_(nullptr), pRotateAction_(nullptr),
	pVisionSearchAction_(nullptr), boneIndex_(-1), partIndex_(-1)
{
}

Feet::~Feet()
{
}

void Feet::Initialize()
{
	hModel_ = Model::Load("Model/stoneGolem.fbx");
	assert(hModel_ >= 0);

	CreateStage* pCreateStage = GameManager::GetCreateStage();
	XMFLOAT3 startPos = pCreateStage->GetRandomFloarPosition();
	transform_.position_ = startPos;
	transform_.rotate_.y = (float)(rand() % 360);

	maxHp_ = 100;
	hp_ = maxHp_;
	aimTargetPos_ = 1.0f;
	bodyWeight_ = 10.0f;
	attackDamage_ = 50;

	//Colliderの設定
	SphereCollider* collision1 = new SphereCollider(XMFLOAT3(0, 0.5, 0), 0.75f);
	SphereCollider* collision2 = new SphereCollider(XMFLOAT3(0, 1.5, 0), 0.75f);
	pHandCollider_ = new SphereCollider(XMFLOAT3(0, 0, 0), 0.5f);
	pHandCollider_->SetValid(false);
	AddCollider(collision1);
	AddCollider(collision2);
	AddAttackCollider(pHandCollider_);

	pEnemyUi_ = new EnemyUi(this);
	pEnemyUi_->Initialize(2.5f);

	//Actionの設定
	pMoveAction_ = new AstarMoveAction(this, 0.0f, 0.3f);
	pRotateAction_ = new RotateAction(this, 0.07f);
	pVisionSearchAction_ = new VisionSearchAction(this, 30.0f, 90.0f);
	pRotateAction_->Initialize();

	//ステートの設定
	pStateManager_ = new StateManager(this);
	pStateManager_->AddState(new FeetAppear(pStateManager_));
	pStateManager_->AddState(new FeetPatrol(pStateManager_));
	pStateManager_->AddState(new FeetCombat(pStateManager_));
	pStateManager_->AddState(new FeetDead(pStateManager_));
	pStateManager_->ChangeState("Appear");
	pStateManager_->Initialize();
	
	//CombatStateの設定
	pCombatStateManager_ = new StateManager(this);
	pCombatStateManager_->AddState(new FeetWait(pCombatStateManager_));
	pCombatStateManager_->AddState(new FeetMove(pCombatStateManager_));
	pCombatStateManager_->AddState(new FeetAttack(pCombatStateManager_));
	pCombatStateManager_->ChangeState("Wait");
	pCombatStateManager_->Initialize();
	
	Model::GetBoneIndex(hModel_, "attack_Hand.R", &boneIndex_, &partIndex_);
	assert(boneIndex_ >= 0);

}

void Feet::Update()
{
	GameManager::GetCollisionMap()->CalcMapWall(transform_.position_, 0.3f);
	pStateManager_->Update();

}

void Feet::Draw()
{
	pEnemyUi_->Draw();

	XMFLOAT3 center = Model::GetBoneAnimPosition(hModel_, boneIndex_, partIndex_);
	center = XMFLOAT3(center.x - transform_.position_.x, center.y - transform_.position_.y, center.z - transform_.position_.z);
	pHandCollider_->SetCenter(center);

	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	//デバッグ用
	CollisionDraw();

}

void Feet::Release()
{
	SAFE_DELETE(pVisionSearchAction_);
	SAFE_DELETE(pRotateAction_);
	SAFE_DELETE(pMoveAction_);

	EnemyBase::Release();
	Model::Release(hModel_);

}

void Feet::ApplyDamage(int da)
{
	EnemyBase::ApplyDamage(da);

	if (pStateManager_->GetName() != "Combat") {
		pStateManager_->ChangeState("Combat");
	}

}

void Feet::OnCollision(GameObject* pTarget)
{
	std::string name = pTarget->GetObjectName();
	if (name.find("Enemy") != std::string::npos || name == "Player") {
		Character* c = static_cast<Character*>(pTarget);
		ReflectCharacter(c);
	}

}
#include "ThrowEnemy.h"
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

ThrowEnemy::ThrowEnemy(GameObject* parent)
	: EnemyBase(parent, "ThrowEnemy"), hModel_(-1), pMoveAction_(nullptr), pRotateAction_(nullptr), pVisionSearchAction_(nullptr),
	boneIndex_(-1), partIndex_(-1) 
{
}

ThrowEnemy::~ThrowEnemy()
{
}

void ThrowEnemy::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Model/stoneGolem.fbx");
	assert(hModel_ >= 0);

	CreateStage* pCreateStage = GameManager::GetCreateStage();
	XMFLOAT3 startPos = pCreateStage->GetRandomFloarPosition();
	transform_.position_ = startPos;
	transform_.rotate_.y = (float)(rand() % 360);
	transform_.scale_ = { 0.5f, 0.5f, 0.5f };

	maxHp_ = 100;
	hp_ = maxHp_;
	aimTargetPos_ = 1.0f;
	bodyWeight_ = 10.0f;
	attackDamage_ = 50;

	//Colliderの設定
	SphereCollider* collision1 = new SphereCollider(XMFLOAT3(0, 0.25, 0), 0.325f);
	SphereCollider* collision2 = new SphereCollider(XMFLOAT3(0, 0.75, 0), 0.325f);
	AddCollider(collision1);
	AddCollider(collision2);

	pEnemyUi_ = new EnemyUi(this);
	pEnemyUi_->Initialize(1.25f);

	//Actionの設定
	pMoveAction_ = new AstarMoveAction(this, 0.0f, 0.3f);
	pRotateAction_ = new RotateAction(this, 0.07f);
	pVisionSearchAction_ = new VisionSearchAction(this, 30.0f, 90.0f);
	pRotateAction_->Initialize();

	Model::GetBoneIndex(hModel_, "attack_Hand.R", &boneIndex_, &partIndex_);
	assert(boneIndex_ >= 0);
}

void ThrowEnemy::Update()
{
	GameManager::GetCollisionMap()->CalcMapWall(transform_.position_, 0.3f);
	pStateManager_->Update();

}

void ThrowEnemy::Draw()
{
	pEnemyUi_->Draw();

	XMFLOAT3 center = Model::GetBoneAnimPosition(hModel_, boneIndex_, partIndex_);
	center = XMFLOAT3(center.x - transform_.position_.x, center.y - transform_.position_.y, center.z - transform_.position_.z);
	
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	//デバッグ用
	CollisionDraw();

}

void ThrowEnemy::Release()
{
	SAFE_DELETE(pVisionSearchAction_);
	SAFE_DELETE(pRotateAction_);
	SAFE_DELETE(pMoveAction_);

	EnemyBase::Release();
	Model::Release(hModel_);

}

void ThrowEnemy::ApplyDamage(int da)
{
	EnemyBase::ApplyDamage(da);

	//if (pStateManager_->GetName() != "Combat") {
	//	pStateManager_->ChangeState("Combat");
	//}

}

void ThrowEnemy::OnCollision(GameObject* pTarget)
{
	std::string name = pTarget->GetObjectName();
	if (pTarget->GetObjectName().find("Enemy") != std::string::npos || name == "Player") {
		Character* c = static_cast<Character*>(pTarget);
		ReflectCharacter(c);
	}

}

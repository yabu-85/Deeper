#include "ThrowEnemy.h"
#include "EnemyUi.h"
#include "../GameManager/GameManager.h"
#include "../State/ThrowState.h"
#include "../Engine/Model.h"
#include "../Engine/SphereCollider.h"
#include "../Engine/Global.h"
#include "../State/StateManager.h"
#include "../Stage/CreateStage.h"
#include "../Stage/CollisionMap.h"
#include "../Player/Player.h"
#include "../Player/LifeManager.h"
#include "../Weapon/ThrowBullet.h"
#include "../Weapon/NormalBullet.h"

#include "../Action/MoveAction.h"
#include "../Action/RotateAction.h"
#include "../Action/SearchAction.h"

ThrowEnemy::ThrowEnemy(GameObject* parent)
	: EnemyBase(parent, "ThrowEnemy"), hModel_(-1), hItemModel_(-1), boneIndex_(-1), partIndex_(-1),
	pMoveAction_(nullptr), pRotateAction_(nullptr), pVisionSearchAction_(nullptr), isHasItem_(true)
{
}

ThrowEnemy::~ThrowEnemy()
{
}

void ThrowEnemy::Initialize()
{
	hModel_ = Model::Load("Model/stoneGolem.fbx");
	assert(hModel_ >= 0);
	hItemModel_ = Model::Load("Model/RedBox.fbx");
	assert(hItemModel_ >= 0);

	CreateStage* pCreateStage = GameManager::GetCreateStage();
	XMFLOAT3 startPos = pCreateStage->GetRandomFloarPosition();
	transform_.position_ = startPos;
	transform_.rotate_.y = (float)(rand() % 360);
	transform_.scale_ = { 0.5f, 0.5f, 0.5f };
	itemTransform_.scale_ = { 0.2f, 0.2f, 0.2f };

	type_ = ENEMY_THROW;
	maxHp_ = 30;
	hp_ = maxHp_;
	aimTargetPos_ = 1.1f;
	bodyWeight_ = 10.0f;
	bodyRange_ = 0.3f;
	attackDamage_ = 1;
	combatDistance_ = 7.0f;
	attackDistance_ = 8.0f;

	//Colliderの設定
	SphereCollider* collision1 = new SphereCollider(XMFLOAT3(0, 0.7f, 0), 0.6f);
	AddCollider(collision1);

	pEnemyUi_ = new EnemyUi(this);
	pEnemyUi_->Initialize(1.0f);

	//Actionの設定
	pMoveAction_ = new AstarMoveAction(this, 0.0f, 0.3f);
	pRotateAction_ = new RotateAction(this, 0.07f);
	pVisionSearchAction_ = new VisionSearchAction(this, 30.0f, 90.0f);
	pRotateAction_->Initialize();

	//ステートの設定
	pStateManager_ = new StateManager(this);
	pStateManager_->AddState(new ThrowAppear(pStateManager_));
	pStateManager_->AddState(new ThrowDead(pStateManager_));
	pStateManager_->AddState(new ThrowPatrol(pStateManager_));
	pStateManager_->AddState(new ThrowCombat(pStateManager_));
	pStateManager_->ChangeState("Appear");
	pStateManager_->Initialize();

	//CombatStateの設定
	pCombatStateManager_ = new StateManager(this);
	pCombatStateManager_->AddState(new ThrowWait(pCombatStateManager_));
	pCombatStateManager_->AddState(new ThrowMove(pCombatStateManager_));
	pCombatStateManager_->AddState(new ThrowAttack(pCombatStateManager_));
	pCombatStateManager_->ChangeState("Wait");
	pCombatStateManager_->Initialize();

	Model::GetBoneIndex(hModel_, "attack_Hand.R", &boneIndex_, &partIndex_);
	assert(boneIndex_ >= 0);
}

void ThrowEnemy::Update()
{
	EnemyBase::Update();

	pStateManager_->Update();
	GameManager::GetCollisionMap()->CalcMapWall(transform_.position_, 0.3f, GetBodyRange());

}

void ThrowEnemy::Draw()
{
	pEnemyUi_->Draw();

	if (isHasItem_) {
		itemTransform_.position_ = Model::GetBoneAnimPosition(hModel_, boneIndex_, partIndex_);
		Model::SetTransform(hItemModel_, itemTransform_);
		Model::Draw(hItemModel_);
	}
	
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
	//死んでたら終わり
	if (pStateManager_->GetName() == "Dead") return;

	if (pStateManager_->GetName() != "Combat") {
		pStateManager_->ChangeState("Combat");
	}

}

void ThrowEnemy::OnAttackCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player") {
		GameManager::GetPlayer()->TargetRotate(GetPosition());
		LifeManager::Damage(GetAttackDamage());
	}
}

void ThrowEnemy::ThrowItem()
{
	isHasItem_ = false;
	
	XMFLOAT3 plaPos = GameManager::GetPlayer()->GetPosition();
	plaPos.y += 0.75f;

	NormalBullet* bullet = Instantiate<NormalBullet>(GetParent());
	bullet->Shot(itemTransform_.position_, plaPos);
	bullet->SetShotParent(this);
	bullet->SetObjectName("NormalEBullet");
}

void ThrowEnemy::SetItem()
{
	isHasItem_ = true;
}
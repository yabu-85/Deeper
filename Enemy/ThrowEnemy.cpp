#include "ThrowEnemy.h"
#include "EnemyUi.h"
#include "../State/ThrowState.h"
#include "../Engine/Model.h"
#include "../Engine/SphereCollider.h"
#include "../State/StateManager.h"
#include "../Stage/CreateStage.h"
#include "../Engine/Global.h"
#include "../GameManager.h"
#include "../Stage/CollisionMap.h"
#include "../Player/Player.h"
#include "../Weapon/ThrowBullet.h"

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

	maxHp_ = 100;
	hp_ = maxHp_;
	aimTargetPos_ = 1.0f;
	bodyWeight_ = 10.0f;
	bodyRange_ = 0.25f;
	attackDamage_ = 50;

	//Collider�̐ݒ�
	SphereCollider* collision1 = new SphereCollider(XMFLOAT3(0, 0.25, 0), 0.325f);
	SphereCollider* collision2 = new SphereCollider(XMFLOAT3(0, 0.75, 0), 0.325f);
	AddCollider(collision1);
	AddCollider(collision2);

	pEnemyUi_ = new EnemyUi(this);
	pEnemyUi_->Initialize(1.0f);

	//Action�̐ݒ�
	pMoveAction_ = new AstarMoveAction(this, 0.0f, 0.3f);
	pRotateAction_ = new RotateAction(this, 0.07f);
	pVisionSearchAction_ = new VisionSearchAction(this, 30.0f, 90.0f);
	pRotateAction_->Initialize();

	//�X�e�[�g�̐ݒ�
	pStateManager_ = new StateManager(this);
	pStateManager_->AddState(new ThrowAppear(pStateManager_));
	pStateManager_->AddState(new ThrowPatrol(pStateManager_));
	pStateManager_->AddState(new ThrowCombat(pStateManager_));
	pStateManager_->AddState(new ThrowDead(pStateManager_));
	pStateManager_->ChangeState("Appear");
	pStateManager_->Initialize();

	//CombatState�̐ݒ�
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
	pStateManager_->Update();
	GameManager::GetCollisionMap()->CalcMapWall(transform_.position_, 0.3f);

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

	//�f�o�b�O�p
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

	if (pStateManager_->GetName() != "Combat") {
		pStateManager_->ChangeState("Combat");
	}

}

void ThrowEnemy::OnCollision(GameObject* pTarget)
{
	std::string name = pTarget->GetObjectName();
	if (pTarget->GetObjectName().find("Enemy") != std::string::npos || name == "Player") {
		Character* c = static_cast<Character*>(pTarget);
		ReflectCharacter(c);
	}

}

void ThrowEnemy::ThrowItem()
{
	isHasItem_ = false;
	
	XMFLOAT3 plaPos = GameManager::GetPlayer()->GetPosition();
	plaPos.y += 0.75f;

	ThrowBullet* bullet = Instantiate<ThrowBullet>(GetParent());
	bullet->Shot(itemTransform_.position_, plaPos);

}

void ThrowEnemy::SetItem()
{
	isHasItem_ = true;
}

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

#include "../Action/MoveAction.h"
#include "../Action/RotateAction.h"
#include "../Action/SearchAction.h"

#include "../Engine/Input.h"

MeleeFighter::MeleeFighter(GameObject* parent)
	: EnemyBase(parent, "MeleeFighterEnemy"), hModel_(-1), pHandCollider_{ nullptr, nullptr }, pMoveAction_(nullptr), pRotateAction_(nullptr),
	pVisionSearchAction_(nullptr), pOrientedMoveAction_(nullptr), boneIndex_{ -1,-1 }, partIndex_{ -1,-1 }, pDamageController_(nullptr),
	pAnimationController_(nullptr), pPolyLine_(nullptr)
{
}

MeleeFighter::~MeleeFighter()
{
}

void MeleeFighter::Initialize()
{
	hModel_ = Model::Load("Model/walf.fbx");
	assert(hModel_ >= 0);

	CreateStage* pCreateStage = GameManager::GetCreateStage();
	XMFLOAT3 startPos = pCreateStage->GetRandomFloarPosition();
	transform_.position_ = startPos;
	transform_.rotate_.y = (float)(rand() % 360);

	SetHP(100);
	SetMaxHP(100);
	SetBodyWeight(100.0f);
	SetBodyRange(0.7f);

	type_ = ENEMY_MELEE;
	aimTargetPos_ = 1.3f;
	attackDistance_ = 2.0f;
	combatDistance_ = 5.0f;

	//アニメーションデータのセットフレームはヘッダに書いてる
	pAnimationController_ = new AnimationController(hModel_);
	for (int i = 0; i < (int)MELEE_ANIMATION::MAX; i++) pAnimationController_->AddAnime(MELEE_ANIMATION_DATA[i].startFrame, MELEE_ANIMATION_DATA[i].endFrame);

	//Colliderの設定
	SphereCollider* collision1 = new SphereCollider(XMFLOAT3(0, 0.5, 0), 0.5f);
	SphereCollider* collision2 = new SphereCollider(XMFLOAT3(0, 1.5, 0), 0.5f);
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

	pDamageController_ = new DamageController();

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
	assert(boneIndex_[0] >= 0);
	Model::GetBoneIndex(hModel_, "forearm.R", &boneIndex_[1], &partIndex_[1]);
	assert(boneIndex_[1] >= 0);

	pPolyLine_ = new PolyLine;
	pPolyLine_->Load("PolyImage/Sword.png");
	pPolyLine_->SetLength(30);

	DamageInfo damage(this, "MeleeAttack", 3);
	KnockBackInfo knockBack(KNOCK_TYPE::MEDIUM, 3, 0.1f, transform_.position_);
	SetAllAttackColliderValid(true);
	GetDamageController()->SetCurrentDamage(damage);
	GetDamageController()->SetCurrentKnockBackInfo(knockBack);

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
	pPolyLine_->Draw();

	//ColliderPosition
	for (int i = 0; i < 2; i++) {
		XMFLOAT3 center = Model::GetBoneAnimPosition(hModel_, boneIndex_[i], partIndex_[i]);
		center = XMFLOAT3(center.x - transform_.position_.x, center.y - transform_.position_.y, center.z - transform_.position_.z);
		pHandCollider_[i]->SetCenter(center);
	}	

	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	CollisionDraw();
	pEnemyUi_->Draw();

}

void MeleeFighter::Release()
{
	SAFE_RELEASE(pPolyLine_);
	SAFE_DELETE(pPolyLine_); 
	SAFE_DELETE(pVisionSearchAction_);
	SAFE_DELETE(pRotateAction_);
	SAFE_DELETE(pMoveAction_);

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

		//攻撃入ったらリストに追加
		if (p->ApplyDamageWithList(damage, knock)) {
			GetDamageController()->AddAttackList(p);
		}
	}
}

void MeleeFighter::CalcPoly()
{
	XMFLOAT3 wandPos_ = Model::GetBoneAnimPosition(hModel_, boneIndex_[0], partIndex_[0]);
	XMFLOAT3 rotate_ = Model::GetBoneAnimRotate(hModel_, boneIndex_[0], partIndex_[0]);
	
	XMFLOAT3 tar = XMFLOAT3(rotate_.x, rotate_.y, 0.0f);
	XMFLOAT3 target;
	target.x = (float)sin(XMConvertToRadians(tar.y));
	target.y = -(float)tan(XMConvertToRadians(tar.x));
	target.z = (float)cos(XMConvertToRadians(tar.y));

	if (tar.x >= 90.0f || tar.x <= -90.0f) {
		target.x *= -1.0f;
		target.y *= -1.0f;
		target.z *= -1.0f;
	}

	XMFLOAT3 vec = target;
	XMVECTOR vVec = XMLoadFloat3(&vec);
	vVec = XMVector3Normalize(vVec);
	XMStoreFloat3(&vec, vVec);

	vec = XMFLOAT3(wandPos_.x + vec.x, wandPos_.y + vec.y, wandPos_.z + vec.z);
	pPolyLine_->AddPosition(wandPos_, vec);
}

void MeleeFighter::DamageInfoReset()
{
	pPolyLine_->ResetPosition();

	SetAllAttackColliderValid(false);
	GetDamageController()->ResetAttackList();
}

void MeleeFighter::SetDamageInfoCombo1()
{
	DamageInfo damage(this, "StoneArm", 3);
	KnockBackInfo knockBack(KNOCK_TYPE::MEDIUM, 5, 0.1f, transform_.position_);
	SetAllAttackColliderValid(true);
	GetDamageController()->SetCurrentDamage(damage);
	GetDamageController()->SetCurrentKnockBackInfo(knockBack);
}
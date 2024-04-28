#include "Scarecrow.h"
#include "EnemyUi.h"
#include "../GameManager/GameManager.h"
#include "../Engine/Model.h"
#include "../Engine/SphereCollider.h"
#include "../Engine/Global.h"
#include "../State/StateManager.h"
#include "../Stage/CreateStage.h"
#include "../Stage/CollisionMap.h"
#include "../Action/MoveAction.h"

namespace {
	static const XMFLOAT3 POSITION = XMFLOAT3(14.5f, 0.0f, 13.0f);
}

Scarecrow::Scarecrow(GameObject* parent)
	: EnemyBase(parent, "ScarecrowEnemy"), hModel_(-1)
{
}

Scarecrow::~Scarecrow()
{
}

void Scarecrow::Initialize()
{
	hModel_ = Model::Load("Model/Scarecrow.fbx");
	assert(hModel_ >= 0);

	transform_.position_ = POSITION;
	transform_.rotate_.y = 0.0f;

	SetHP(200);
	SetMaxHP(200);
	SetBodyWeight(100.0f);
	SetBodyRange(0.7f);
	
	type_ = ENEMY_STONEGOLEM;
	aimTargetPos_ = 1.2f;
	attackDistance_ = 2.0f;
	combatDistance_ = 5.0f;

	//Colliderの設定
	SphereCollider* collision1 = new SphereCollider(XMFLOAT3(0, 0.3f, 0), 0.5f);
	SphereCollider* collision2 = new SphereCollider(XMFLOAT3(0, 1.1f, 0), 0.5f);
	AddCollider(collision1);
	AddCollider(collision2);

	pEnemyUi_ = new EnemyUi(this);
	pEnemyUi_->Initialize(2.8f);

	pMoveAction_ = new MoveAction(this, 0.01f, 0.1f);
	pMoveAction_->SetTarget(POSITION);

	//ステートの設定
	pStateManager_ = new StateManager(this);
	pCombatStateManager_ = new StateManager(this);
}

void Scarecrow::Update()
{
	EnemyBase::Update();
	GameManager::GetCollisionMap()->CalcMapWall(transform_.position_, 0.1f, GetBodyRange());
	
	pMoveAction_->Update();
	if (GetHP() < GetMaxHP()) {
		SetHP((GetHP() + 1));
		pEnemyUi_->SetParcent((float)(GetHP()) / (float)(GetMaxHP()));
	}
}

void Scarecrow::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	CollisionDraw();
	pEnemyUi_->Draw();
}

void Scarecrow::Release()
{
	EnemyBase::Release();
	Model::Release(hModel_);
	SAFE_DELETE(pMoveAction_);
}

void Scarecrow::Dead()
{
	DeadEnter();
	DeadExit();
}

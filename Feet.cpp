#include "Feet.h"
#include "Engine/Model.h"
#include "EnemyUi.h"
#include "Engine/SphereCollider.h"
#include "StateManager.h"
#include "FeetState.h"
#include "CreateStage.h"
#include "Engine/Global.h"
#include "GameManager.h"
#include "CollisionMap.h"

#include "MoveAction.h"
#include "RotateAction.h"
#include "SearchAction.h"
#include "Engine/Input.h"

Feet::Feet(GameObject* parent)
	:EnemyBase(parent), hModel_(-1), pHandCollider_(nullptr), pMoveAction_(nullptr), pRotateAction_(nullptr), pVisionSearchAction_(nullptr),
	pAuditorySearchAction_(nullptr)
{
	objectName_ = "Feet";
}

Feet::~Feet()
{
}

void Feet::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Model/stoneGolem.fbx");
	assert(hModel_ >= 0);

	maxHp_ = 100;
	hp_ = maxHp_;
	aimTargetPos_ = 2.0f;

	//Colliderの設定
	SphereCollider* collision1 = new SphereCollider(XMFLOAT3(0, 1, 0), 1.5f);
	SphereCollider* collision2 = new SphereCollider(XMFLOAT3(0, 3, 0), 1.5f);
	pHandCollider_ = new SphereCollider(XMFLOAT3(0, 0, 0), 1.0f);
	pHandCollider_->SetAttackCollider(true);
	pHandCollider_->SetValid(false);
	AddCollider(collision1);
	AddCollider(collision2);
	AddCollider(pHandCollider_);

	pEnemyUi_ = new EnemyUi(this);
	pEnemyUi_->Initialize(5.0f);

	//Actionの設定
	pMoveAction_ = new AstarMoveAction(this, 0.05f, 2.0f);
	pRotateAction_ = new RotateAction(this, 0.07f);
	pVisionSearchAction_ = new VisionSearchAction(this, 30.0f / floarSize, 90.0f);
	pAuditorySearchAction_ = new AuditorySearchAction(this);
	pRotateAction_->Initialize();

	//ステートの設定
	pStateManager_ = new StateManager(this);
	pStateManager_->AddState(new FeetAppear(pStateManager_));
	pStateManager_->AddState(new FeetIdle(pStateManager_));
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

	CreateStage* pCreateStage = GameManager::GetCreateStage();
	XMFLOAT3 startPos = pCreateStage->GetRandomFloarPosition();
	transform_.position_ = startPos;
	transform_.rotate_.y = (float)(rand() % 360);

	bodyWeight_ = 10.0f;

}

void Feet::Update()
{
	XMVECTOR prePos = XMLoadFloat3(&transform_.position_);


	CollisionMap* pMap = static_cast<CollisionMap*>(FindObject("CollisionMap"));
	pMap->CalcMapWall(transform_.position_, 1.0f);

	if(pStateManager_) pStateManager_->Update();

	movement_ = prePos - XMLoadFloat3(&transform_.position_);

}

void Feet::Draw()
{
	if (pEnemyUi_) pEnemyUi_->Draw();

	if (pHandCollider_) {
		XMFLOAT3 center = Model::GetBoneAnimPosition(hModel_, "hand.R");
		center = XMFLOAT3(center.x - transform_.position_.x, center.y - transform_.position_.y, center.z - transform_.position_.z);
		pHandCollider_->SetCenter(center);
	}

	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	CollisionDraw();

	if (Input::IsKey(DIK_F)) {
		Transform target;
		target.scale_ = XMFLOAT3(0.2f, 0.2f, 0.2f);
		std::vector<XMFLOAT3> targetList = pMoveAction_->GetTarget();
		if (targetList.empty()) return;
		for (auto pos : targetList) {
			target.position_ = XMFLOAT3(pos.x * floarSize + floarSize / 2.0f, pos.y, pos.z * floarSize + floarSize / 2.0f);
			target.position_.y += 1.0f;
			Model::SetTransform(hModel_, target);
			Model::Draw(hModel_);
		}
	}
}

void Feet::Release()
{
	EnemyBase::Release();

	SAFE_DELETE(pAuditorySearchAction_);
	SAFE_DELETE(pVisionSearchAction_);
	SAFE_DELETE(pRotateAction_);
	SAFE_DELETE(pMoveAction_);
	SAFE_DELETE(pEnemyUi_);
	SAFE_DELETE(pCombatStateManager_);
	SAFE_DELETE(pStateManager_);

}

void Feet::ApplyDamage(int da)
{
	EnemyBase::ApplyDamage(da);

	if (pStateManager_->GetName() != "Combat") {
		pStateManager_->ChangeState("Combat");
	}

}

void Feet::OnAttackCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player") pHandCollider_->SetValid(false);

}

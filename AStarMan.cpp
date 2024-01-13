#include "AStarMan.h"
#include "Engine/Model.h"
#include "CreateStage.h"
#include "Engine/Global.h"
#include "GameManager.h"
#include "CreateStage.h"

#include "MoveAction.h"
#include "RotateAction.h"

//デバッグ用
#include <vector>
#include "Engine/Input.h"
#include "Player.h"

AStarMan::AStarMan(GameObject* parent)
	:EnemyBase(parent), hModel_(-1), pAstarMoveAction_(nullptr)
{
}

AStarMan::~AStarMan()
{
}

void AStarMan::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Model/SphereCollider.fbx");
	assert(hModel_ >= 0);
	
	aimTargetPos_ = 1.0f;

	CreateStage* pCreateStage = GameManager::GetCreateStage();
	XMFLOAT3 startPos = pCreateStage->GetRandomFloarPosition();
	transform_.position_ = startPos;
	transform_.scale_ = XMFLOAT3(0.5f, 0.5f, 0.5f);

	pAstarMoveAction_ = new AstarMoveAction(this, 0.03f, 0.1f);
	pAstarMoveAction_->Initialize();

}

void AStarMan::Update()
{
	if (pAstarMoveAction_->IsInRange() && rand() % 60 == 0) {
		CreateStage* pCreateStage = GameManager::GetCreateStage();
		pAstarMoveAction_->SetTarget(pCreateStage->GetRandomFloarPosition());
	}

	if (rand() % 10 == 0 && pAstarMoveAction_->IsOutEndTarget()) {
		Player* pPlayer = static_cast<Player*>(FindObject("Player"));
		pAstarMoveAction_->UpdatePath(pPlayer->GetPosition());
	}

	if (Input::IsKey(DIK_3)) {
		CreateStage* pCreateStage = GameManager::GetCreateStage();
		pAstarMoveAction_->UpdatePath(pCreateStage->GetFloarPosition(transform_.position_, 20.0f));
	}
	
	if (Input::IsKey(DIK_4)) {
		CreateStage* pCreateStage = GameManager::GetCreateStage();
		pAstarMoveAction_->UpdatePath(pCreateStage->GetFloarPosition(transform_.position_, 0.0f));
	}
	
	pAstarMoveAction_->Update();

}

void AStarMan::Draw()
{
	Transform draw = transform_;
	draw.position_.y += 1.0f;
	Model::SetTransform(hModel_, draw);
	Model::Draw(hModel_);

	if (!Input::IsKey(DIK_F)) {
		Transform target;
		target.scale_ = XMFLOAT3(0.2f, 0.2f, 0.2f);
		std::vector<XMFLOAT3> targetList = pAstarMoveAction_->GetTarget();
		if (targetList.empty()) return;
		for (auto pos : targetList) {
			target.position_ = XMFLOAT3(pos.x * floarSize + floarSize / 2.0f, pos.y, pos.z * floarSize + floarSize / 2.0f);
			target.position_.y += 1.0f;
			Model::SetTransform(hModel_, target);
			Model::Draw(hModel_);
		}
	}
}

void AStarMan::Release()
{
	SAFE_DELETE(pAstarMoveAction_);

}

#include "AStarMan.h"
#include "Engine/Model.h"
#include "NavigationAI.h"
#include "EnemyUi.h"
#include "Engine/SphereCollider.h"
#include "StateManager.h"
#include "FeetState.h"

#include "MoveAction.h"
#include "RotateAction.h"

#include <vector>
#include "Stage.h"
#include "GameManager.h"
namespace {

}

AStarMan::AStarMan(GameObject* parent)
	:EnemyBase(parent), hModel_(-1), targetPos_{0.0f, 0.0f, 0.0f}
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
	
	transform_.position_ = XMFLOAT3(11.0f, 0.0f, 12.0f);
	targetPos_ = XMFLOAT3(10.0f, 0.0f, 22.0f);

}

void AStarMan::Update()
{
	XMFLOAT3 target = GameManager::GetNavigationAI()->Navi(targetPos_, transform_.position_);

	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
	XMVECTOR vTar = XMLoadFloat3(&target);
	XMVECTOR vMove = vTar - vPos;
	vMove = XMVector3Normalize(vMove) * 0.02f;
	XMStoreFloat3(&transform_.position_, vPos + vMove);
	
}

void AStarMan::Draw()
{
	Transform draw = transform_;
	draw.position_.y += 1.0f;
	Model::SetTransform(hModel_, draw);
	Model::Draw(hModel_);

	Transform target;
	target.position_ = targetPos_;
	target.position_.y += 1.0f;
	Model::SetTransform(hModel_, target);
	Model::Draw(hModel_);

}

void AStarMan::Release()
{
}

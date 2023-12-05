#include "MoveAction.h"
#include "Player.h"

MoveAction::MoveAction(GameObject* obj) : BaseAction(obj), isInRange_(false), moveSpeed_(0.1f), moveRange_(1.0f), targetPos_(0,0,0)
{
}

MoveAction::MoveAction(GameObject* obj, float speed, float range)
	: BaseAction(obj), isInRange_(false), moveSpeed_(speed), moveRange_(range), targetPos_(0, 0, 0)
{
}

void MoveAction::Update()
{
	XMFLOAT3 pos = pGameObject_->GetPosition();
	XMVECTOR vPos = XMLoadFloat3(&pos);
	XMVECTOR vTar = XMLoadFloat3(&targetPos_);
	XMVECTOR vMove = vTar - vPos;
	float currentSpeed = XMVectorGetX(XMVector3Length(vTar - vPos));
	if(currentSpeed > moveSpeed_) vMove = XMVector3Normalize(vMove) * moveSpeed_;

	//Target位置ついた
	float length = XMVectorGetX(XMVector3Length(vTar - vPos));
	if (length <= moveRange_) {
		isInRange_ = true;
	}

	XMStoreFloat3(&pos, vPos + vMove);
	pGameObject_->SetPosition(pos);
	isInRange_ = false;;
}

//------------------------------Astar----------------------

#include "GameManager.h"
#include "NavigationAI.h"

namespace {
	float floarSize = 1.0f;
}

AstarMoveAction::AstarMoveAction(GameObject* obj) : MoveAction(obj)
{
}

AstarMoveAction::AstarMoveAction(GameObject* obj, float speed, float range) : MoveAction(obj, speed, range)
{
}

AstarMoveAction::~AstarMoveAction()
{
}

void AstarMoveAction::Update()
{
	isInRange_ = false;

	//移動終了した
	if (targetList_.empty()) {
		isInRange_ = true;
		return;
	}

	XMFLOAT3 pos = pGameObject_->GetPosition();
	XMVECTOR vPos = XMLoadFloat3(&pos);
	XMVECTOR vTar = XMLoadFloat3(&targetList_.back()) * floarSize;
	XMVECTOR vMove = vTar - vPos;
	float currentSpeed = XMVectorGetX(XMVector3Length(vTar - vPos));
	if (currentSpeed > moveSpeed_ * floarSize) vMove = XMVector3Normalize(vMove) * (moveSpeed_ * floarSize);

	//Target位置ついた：カクカクしないように再起処理する
	float length = XMVectorGetX(XMVector3Length(vTar - vPos));
	if (length <= moveRange_) {
		targetList_.pop_back();
		Update();
		return;
	}

	XMStoreFloat3(&pos, vPos + vMove);
	pGameObject_->SetPosition(pos);
}

void AstarMoveAction::SetTarget(XMFLOAT3 target)
{
	targetList_ = GameManager::GetNavigationAI()->NaviDiagonal(target, pGameObject_->GetPosition());
}
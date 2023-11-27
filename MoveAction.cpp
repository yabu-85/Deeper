#include "MoveAction.h"
#include "Player.h"

MoveAction::MoveAction(GameObject* obj) : BaseAction(obj), isInRange_(false), moveSpeed_(0.1f), moveRange_(1.0f)
{
}

MoveAction::MoveAction(GameObject* obj, float speed, float range)
	: BaseAction(obj), isInRange_(false), moveSpeed_(speed), moveRange_(range)
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

	//TargetˆÊ’u‚Â‚¢‚½
	float length = XMVectorGetX(XMVector3Length(vTar - vPos));
	if (length <= moveRange_) {
		isInRange_ = true;
	}

	XMStoreFloat3(&pos, vPos + vMove);
	pGameObject_->SetPosition(pos);
	isInRange_ = false;;
}

void MoveAction::Initialize()
{
}

void MoveAction::Terminate()
{
}
